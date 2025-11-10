/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file infrastructure_sharing_gui.cpp GUI for infrastructure sharing configuration. */

#include "stdafx.h"
#include "command_func.h"
#include "company_base.h"
#include "company_func.h"
#include "company_extended_func.h"
#include "company_cmd.h"
#include "window_gui.h"
#include "strings_func.h"
#include "widgets/dropdown_type.h"
#include "widgets/dropdown_func.h"

#include "table/strings.h"

#include "safeguards.h"

/** Widgets for infrastructure sharing window. */
enum InfrastructureSharingWidgets {
	WID_IS_CAPTION,           ///< Window caption
	WID_IS_MODE_LABEL,        ///< Label for sharing mode
	WID_IS_MODE_DROPDOWN,     ///< Dropdown for sharing mode
	WID_IS_RAIL_LABEL,        ///< Label for rail fee
	WID_IS_RAIL_VALUE,        ///< Value display for rail fee
	WID_IS_RAIL_DECREASE,     ///< Decrease rail fee button
	WID_IS_RAIL_INCREASE,     ///< Increase rail fee button
	WID_IS_ROAD_LABEL,        ///< Label for road fee
	WID_IS_ROAD_VALUE,        ///< Value display for road fee
	WID_IS_ROAD_DECREASE,     ///< Decrease road fee button
	WID_IS_ROAD_INCREASE,     ///< Increase road fee button
	WID_IS_STATION_LABEL,     ///< Label for station fee
	WID_IS_STATION_VALUE,     ///< Value display for station fee
	WID_IS_STATION_DECREASE,  ///< Decrease station fee button
	WID_IS_STATION_INCREASE,  ///< Increase station fee button
	WID_IS_AIRPORT_LABEL,     ///< Label for airport fee
	WID_IS_AIRPORT_VALUE,     ///< Value display for airport fee
	WID_IS_AIRPORT_DECREASE,  ///< Decrease airport fee button
	WID_IS_AIRPORT_INCREASE,  ///< Increase airport fee button
	WID_IS_APPLY,             ///< Apply button
};

static constexpr std::initializer_list<NWidgetPart> _nested_infrastructure_sharing_widgets = {
	NWidget(NWID_HORIZONTAL),
		NWidget(WWT_CLOSEBOX, COLOUR_GREY),
		NWidget(WWT_CAPTION, COLOUR_GREY, WID_IS_CAPTION), SetDataTip(STR_INFRASTRUCTURE_SHARING_CAPTION, STR_TOOLTIP_WINDOW_TITLE_DRAG_THIS),
		NWidget(WWT_SHADEBOX, COLOUR_GREY),
	EndContainer(),
	NWidget(WWT_PANEL, COLOUR_GREY),
		NWidget(NWID_VERTICAL), SetPIP(8, 4, 8), SetPadding(8),
			/* Sharing mode selection */
			NWidget(NWID_HORIZONTAL), SetPIP(8, 4, 8),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_MODE_LABEL), SetDataTip(STR_INFRASTRUCTURE_SHARING_MODE, STR_NULL), SetFill(1, 0),
				NWidget(WWT_DROPDOWN, COLOUR_GREY, WID_IS_MODE_DROPDOWN), SetMinimalSize(120, 12), SetDataTip(STR_BLACK_STRING, STR_INFRASTRUCTURE_SHARING_MODE_TOOLTIP),
			EndContainer(),
			
			NWidget(NWID_SPACER), SetMinimalSize(0, 4),
			
			/* Rail fee */
			NWidget(NWID_HORIZONTAL), SetPIP(8, 4, 8),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_RAIL_LABEL), SetDataTip(STR_INFRASTRUCTURE_SHARING_RAIL_FEE, STR_NULL), SetFill(1, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_RAIL_DECREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_MINUS, STR_NULL),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_RAIL_VALUE), SetMinimalSize(80, 12), SetDataTip(STR_JUST_CURRENCY_LONG, STR_NULL), SetAlignment(SA_CENTER),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_RAIL_INCREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_PLUS, STR_NULL),
			EndContainer(),
			
			/* Road fee */
			NWidget(NWID_HORIZONTAL), SetPIP(8, 4, 8),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_ROAD_LABEL), SetDataTip(STR_INFRASTRUCTURE_SHARING_ROAD_FEE, STR_NULL), SetFill(1, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_ROAD_DECREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_MINUS, STR_NULL),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_ROAD_VALUE), SetMinimalSize(80, 12), SetDataTip(STR_JUST_CURRENCY_LONG, STR_NULL), SetAlignment(SA_CENTER),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_ROAD_INCREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_PLUS, STR_NULL),
			EndContainer(),
			
			/* Station fee */
			NWidget(NWID_HORIZONTAL), SetPIP(8, 4, 8),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_STATION_LABEL), SetDataTip(STR_INFRASTRUCTURE_SHARING_STATION_FEE, STR_NULL), SetFill(1, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_STATION_DECREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_MINUS, STR_NULL),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_STATION_VALUE), SetMinimalSize(80, 12), SetDataTip(STR_JUST_CURRENCY_LONG, STR_NULL), SetAlignment(SA_CENTER),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_STATION_INCREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_PLUS, STR_NULL),
			EndContainer(),
			
			/* Airport fee */
			NWidget(NWID_HORIZONTAL), SetPIP(8, 4, 8),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_AIRPORT_LABEL), SetDataTip(STR_INFRASTRUCTURE_SHARING_AIRPORT_FEE, STR_NULL), SetFill(1, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_AIRPORT_DECREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_MINUS, STR_NULL),
				NWidget(WWT_TEXT, COLOUR_GREY, WID_IS_AIRPORT_VALUE), SetMinimalSize(80, 12), SetDataTip(STR_JUST_CURRENCY_LONG, STR_NULL), SetAlignment(SA_CENTER),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_AIRPORT_INCREASE), SetMinimalSize(20, 12), SetDataTip(STR_SIGN_PLUS, STR_NULL),
			EndContainer(),
			
			NWidget(NWID_SPACER), SetMinimalSize(0, 4),
			
			/* Apply button */
			NWidget(NWID_HORIZONTAL),
				NWidget(NWID_SPACER), SetFill(1, 0),
				NWidget(WWT_PUSHTXTBTN, COLOUR_GREY, WID_IS_APPLY), SetMinimalSize(100, 12), SetDataTip(STR_INFRASTRUCTURE_SHARING_APPLY, STR_INFRASTRUCTURE_SHARING_APPLY_TOOLTIP),
				NWidget(NWID_SPACER), SetFill(1, 0),
			EndContainer(),
		EndContainer(),
	EndContainer(),
};

/** Infrastructure sharing configuration window. */
struct InfrastructureSharingWindow : Window {
	InfrastructureSharingMode mode;     ///< Current sharing mode
	Money rail_fee;                     ///< Rail fee per tile
	Money road_fee;                     ///< Road fee per tile
	Money station_fee;                  ///< Station fee per use
	Money airport_fee;                  ///< Airport fee per use

	InfrastructureSharingWindow(WindowDesc &desc, CompanyID company) : Window(desc)
	{
		const Company *c = Company::Get(company);
		this->mode = c->infra_sharing_mode;
		this->rail_fee = c->infra_fees.rail_fee_per_tile;
		this->road_fee = c->infra_fees.road_fee_per_tile;
		this->station_fee = c->infra_fees.station_fee_per_use;
		this->airport_fee = c->infra_fees.airport_fee_per_use;
		
		this->InitNested(company);
		this->owner = company;
	}

	void SetStringParameters(WidgetID widget) const override
	{
		switch (widget) {
			case WID_IS_CAPTION:
				SetDParam(0, this->owner);
				break;
			case WID_IS_MODE_DROPDOWN:
				SetDParam(0, STR_INFRASTRUCTURE_SHARING_MODE_DISABLED + this->mode);
				break;
			case WID_IS_RAIL_VALUE:
				SetDParam(0, this->rail_fee);
				break;
			case WID_IS_ROAD_VALUE:
				SetDParam(0, this->road_fee);
				break;
			case WID_IS_STATION_VALUE:
				SetDParam(0, this->station_fee);
				break;
			case WID_IS_AIRPORT_VALUE:
				SetDParam(0, this->airport_fee);
				break;
		}
	}

	void OnClick([[maybe_unused]] Point pt, WidgetID widget, [[maybe_unused]] int click_count) override
	{
		switch (widget) {
			case WID_IS_MODE_DROPDOWN:
				ShowDropDownMenu(this, {STR_INFRASTRUCTURE_SHARING_MODE_DISABLED, STR_INFRASTRUCTURE_SHARING_MODE_ALLOWED, STR_INFRASTRUCTURE_SHARING_MODE_RESTRICTED}, this->mode, WID_IS_MODE_DROPDOWN, 0, 0);
				break;

			case WID_IS_RAIL_DECREASE:
				if (this->rail_fee >= 10) this->rail_fee -= 10;
				this->SetDirty();
				break;

			case WID_IS_RAIL_INCREASE:
				if (this->rail_fee < 10000) this->rail_fee += 10;
				this->SetDirty();
				break;

			case WID_IS_ROAD_DECREASE:
				if (this->road_fee >= 10) this->road_fee -= 10;
				this->SetDirty();
				break;

			case WID_IS_ROAD_INCREASE:
				if (this->road_fee < 10000) this->road_fee += 10;
				this->SetDirty();
				break;

			case WID_IS_STATION_DECREASE:
				if (this->station_fee >= 50) this->station_fee -= 50;
				this->SetDirty();
				break;

			case WID_IS_STATION_INCREASE:
				if (this->station_fee < 100000) this->station_fee += 50;
				this->SetDirty();
				break;

			case WID_IS_AIRPORT_DECREASE:
				if (this->airport_fee >= 100) this->airport_fee -= 100;
				this->SetDirty();
				break;

			case WID_IS_AIRPORT_INCREASE:
				if (this->airport_fee < 200000) this->airport_fee += 100;
				this->SetDirty();
				break;

			case WID_IS_APPLY:
				/* Apply the settings */
				Command<CMD_SET_INFRASTRUCTURE_SHARING>::Post(STR_ERROR_MESSAGE, (uint8_t)this->mode);
				Command<CMD_SET_INFRASTRUCTURE_FEES>::Post(STR_ERROR_MESSAGE, this->rail_fee, this->road_fee, this->station_fee, this->airport_fee);
				this->Close();
				break;
		}
	}

	void OnDropdownSelect(WidgetID widget, int index) override
	{
		if (widget == WID_IS_MODE_DROPDOWN) {
			this->mode = (InfrastructureSharingMode)index;
			this->SetDirty();
		}
	}
};

static WindowDesc _infrastructure_sharing_desc(
	WDP_AUTO, "infrastructure_sharing", 300, 250,
	WC_COMPANY_INFRASTRUCTURE_SHARING, WC_NONE,
	{},
	_nested_infrastructure_sharing_widgets
);

/**
 * Show the infrastructure sharing configuration window.
 * @param company Company to configure infrastructure sharing for.
 */
void ShowInfrastructureSharingWindow(CompanyID company)
{
	if (!Company::IsValidID(company)) return;
	AllocateWindowDescFront<InfrastructureSharingWindow>(_infrastructure_sharing_desc, company);
}
