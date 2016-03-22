#pragma once

namespace Input
{

	enum CelestialKeyCategories{CelestialKeyCategories_CHAR, CelestialKeyCategories_MOD, CelestialKeyCategories_SPEC, CelestialKeyCategories_NA};

	enum CelestialCharKeyCodes{ 
		CelestialCharKeyCodes_TLD
		, CelestialCharKeyCodes_1
		, CelestialCharKeyCodes_2
		, CelestialCharKeyCodes_3
		, CelestialCharKeyCodes_4
		, CelestialCharKeyCodes_5
		, CelestialCharKeyCodes_6
		, CelestialCharKeyCodes_7
		, CelestialCharKeyCodes_8
		, CelestialCharKeyCodes_9
		, CelestialCharKeyCodes_0
		, CelestialCharKeyCodes_PLS
		, CelestialCharKeyCodes_ACT
		, CelestialCharKeyCodes_BCK

		, CelestialCharKeyCodes_TAB
		, CelestialCharKeyCodes_Q
		, CelestialCharKeyCodes_W
		, CelestialCharKeyCodes_E
		, CelestialCharKeyCodes_R
		, CelestialCharKeyCodes_T
		, CelestialCharKeyCodes_Y
		, CelestialCharKeyCodes_U
		, CelestialCharKeyCodes_I
		, CelestialCharKeyCodes_O
		, CelestialCharKeyCodes_P
		, CelestialCharKeyCodes_Å
		, CelestialCharKeyCodes_DRS
		, CelestialCharKeyCodes_ENTR

		, CelestialCharKeyCodes_A
		, CelestialCharKeyCodes_S
		, CelestialCharKeyCodes_D
		, CelestialCharKeyCodes_F
		, CelestialCharKeyCodes_G
		, CelestialCharKeyCodes_H
		, CelestialCharKeyCodes_J
		, CelestialCharKeyCodes_K
		, CelestialCharKeyCodes_L
		, CelestialCharKeyCodes_Ö
		, CelestialCharKeyCodes_Ä
		, CelestialCharKeyCodes_APSTR

		, CelestialCharKeyCodes_ANGL
		, CelestialCharKeyCodes_Z
		, CelestialCharKeyCodes_X
		, CelestialCharKeyCodes_C
		, CelestialCharKeyCodes_V
		, CelestialCharKeyCodes_B
		, CelestialCharKeyCodes_N
		, CelestialCharKeyCodes_M
		, CelestialCharKeyCodes_CMA
		, CelestialCharKeyCodes_DOT
		, CelestialCharKeyCodes_MNS
		, CelestialCharKeyCodes_SPC

		, CelestialCharKeyCodes_NA

	};

	enum CelestialModKeyCodes{ 
		CelestialModKeyCodes_CPS, 
		CelestialModKeyCodes_SHFT,
		CelestialModKeyCodes_CTRL, 
		CelestialModKeyCodes_ALT, 
		CelestialModKeyCodes_ALTGR,
		CelestialModKeyCodes_NA
	};

	enum CelestialSpecKeyCodes
	{
		CelestialSpecKeyCodes_ESC,
		CelestialSpecKeyCodes_F1,
		CelestialSpecKeyCodes_F2,
		CelestialSpecKeyCodes_F3,
		CelestialSpecKeyCodes_F4,

		CelestialSpecKeyCodes_F5,
		CelestialSpecKeyCodes_F6,
		CelestialSpecKeyCodes_F7,
		CelestialSpecKeyCodes_F8,

		CelestialSpecKeyCodes_F9,
		CelestialSpecKeyCodes_F10,
		CelestialSpecKeyCodes_F11,
		CelestialSpecKeyCodes_F12,

		CelestialSpecKeyCodes_PRNTSCRN,
		CelestialSpecKeyCodes_SCRLLCK,
		CelestialSpecKeyCodes_BRK,

		CelestialSpecKeyCodes_INS,
		CelestialSpecKeyCodes_HME,
		CelestialSpecKeyCodes_PGUP,
		CelestialSpecKeyCodes_DEL,
		CelestialSpecKeyCodes_END,
		CelestialSpecKeyCodes_PGDWN,

		CelestialSpecKeyCodes_UP,

		CelestialSpecKeyCodes_WNDS,
		CelestialSpecKeyCodes_MNU,
		CelestialSpecKeyCodes_LFT,
		CelestialSpecKeyCodes_DWN,
		CelestialSpecKeyCodes_RGHT, 
		CelestialSpecKeyCodes_NA

	};
}