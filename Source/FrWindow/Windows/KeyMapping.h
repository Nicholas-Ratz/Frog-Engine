#ifndef KEYMAP_H
#define KEYMAP_H

#include <FrogEngine/Utility.h>

#ifdef FR_WINDOWS

namespace FrogEngine {
    constexpr u8 getKey(const int i) {
        switch (i) {
            case 'A'          : return 0;
            case 'B'          : return 1;
            case 'C'          : return 2;
            case 'D'          : return 3;
            case 'E'          : return 4;
            case 'F'          : return 5;
            case 'G'          : return 6;
            case 'H'          : return 7;
            case 'I'          : return 8;
            case 'J'          : return 9;
            case 'K'          : return 10;
            case 'L'          : return 11;
            case 'M'          : return 12;
            case 'N'          : return 13;
            case 'O'          : return 14;
            case 'P'          : return 15;
            case 'Q'          : return 16;
            case 'R'          : return 17;
            case 'S'          : return 18;
            case 'T'          : return 19;
            case 'U'          : return 20;
            case 'V'          : return 21;
            case 'W'          : return 22;
            case 'X'          : return 23;
            case 'Y'          : return 24;
            case 'Z'          : return 25;
            case '0'          : return 26;
            case '1'          : return 27;
            case '2'          : return 28;
            case '3'          : return 29;
            case '4'          : return 30;
            case '5'          : return 31;
            case '6'          : return 32;
            case '7'          : return 33;
            case '8'          : return 34;
            case '9'          : return 35;
            case VK_SPACE     : return 36;
            case VK_OEM_1     : return 37;
            case VK_OEM_2     : return 38;
            case VK_OEM_3     : return 39;
            case VK_OEM_4     : return 40;
            case VK_OEM_5     : return 41;
            case VK_OEM_6     : return 42;
            case VK_OEM_7     : return 43;
            case VK_OEM_COMMA : return 44;
            case VK_OEM_MINUS : return 45;
            case VK_OEM_PERIOD: return 46;
            case VK_OEM_PLUS  : return 47;
            case VK_CAPITAL   : return 54;
            case VK_TAB       : return 55;
            case VK_RETURN    : return 56;
            case VK_BACK      : return 57;
            case VK_ESCAPE    : return 58;
            case VK_DELETE    : return 59;
            case VK_LEFT      : return 60;
            case VK_RIGHT     : return 61;
            case VK_UP        : return 62;
            case VK_DOWN      : return 63;
            default           : return 0;
        }
    }

    constexpr u8 KEY_MAP[256] = {
        getKey(0),   getKey(1),   getKey(2),   getKey(3),   getKey(4),   getKey(5),   getKey(6),
        getKey(7),   getKey(8),   getKey(9),   getKey(10),  getKey(11),  getKey(12),  getKey(13),
        getKey(14),  getKey(15),  getKey(16),  getKey(17),  getKey(18),  getKey(19),  getKey(20),
        getKey(21),  getKey(22),  getKey(23),  getKey(24),  getKey(25),  getKey(26),  getKey(27),
        getKey(28),  getKey(29),  getKey(30),  getKey(31),  getKey(32),  getKey(33),  getKey(34),
        getKey(35),  getKey(36),  getKey(37),  getKey(38),  getKey(39),  getKey(40),  getKey(41),
        getKey(42),  getKey(43),  getKey(44),  getKey(45),  getKey(46),  getKey(47),  getKey(48),
        getKey(49),  getKey(50),  getKey(51),  getKey(52),  getKey(53),  getKey(54),  getKey(55),
        getKey(56),  getKey(57),  getKey(58),  getKey(59),  getKey(60),  getKey(61),  getKey(62),
        getKey(63),  getKey(64),  getKey(65),  getKey(66),  getKey(67),  getKey(68),  getKey(69),
        getKey(70),  getKey(71),  getKey(72),  getKey(73),  getKey(74),  getKey(75),  getKey(76),
        getKey(77),  getKey(78),  getKey(79),  getKey(80),  getKey(81),  getKey(82),  getKey(83),
        getKey(84),  getKey(85),  getKey(86),  getKey(87),  getKey(88),  getKey(89),  getKey(90),
        getKey(91),  getKey(92),  getKey(93),  getKey(94),  getKey(95),  getKey(96),  getKey(97),
        getKey(98),  getKey(99),  getKey(100), getKey(101), getKey(102), getKey(103), getKey(104),
        getKey(105), getKey(106), getKey(107), getKey(108), getKey(109), getKey(110), getKey(111),
        getKey(112), getKey(113), getKey(114), getKey(115), getKey(116), getKey(117), getKey(118),
        getKey(119), getKey(120), getKey(121), getKey(122), getKey(123), getKey(124), getKey(125),
        getKey(126), getKey(127), getKey(128), getKey(129), getKey(130), getKey(131), getKey(132),
        getKey(133), getKey(134), getKey(135), getKey(136), getKey(137), getKey(138), getKey(139),
        getKey(140), getKey(141), getKey(142), getKey(143), getKey(144), getKey(145), getKey(146),
        getKey(147), getKey(148), getKey(149), getKey(150), getKey(151), getKey(152), getKey(153),
        getKey(154), getKey(155), getKey(156), getKey(157), getKey(158), getKey(159), getKey(160),
        getKey(161), getKey(162), getKey(163), getKey(164), getKey(165), getKey(166), getKey(167),
        getKey(168), getKey(169), getKey(170), getKey(171), getKey(172), getKey(173), getKey(174),
        getKey(175), getKey(176), getKey(177), getKey(178), getKey(179), getKey(180), getKey(181),
        getKey(182), getKey(183), getKey(184), getKey(185), getKey(186), getKey(187), getKey(188),
        getKey(189), getKey(190), getKey(191), getKey(192), getKey(193), getKey(194), getKey(195),
        getKey(196), getKey(197), getKey(198), getKey(199), getKey(200), getKey(201), getKey(202),
        getKey(203), getKey(204), getKey(205), getKey(206), getKey(207), getKey(208), getKey(209),
        getKey(210), getKey(211), getKey(212), getKey(213), getKey(214), getKey(215), getKey(216),
        getKey(217), getKey(218), getKey(219), getKey(220), getKey(221), getKey(222), getKey(223),
        getKey(224), getKey(225), getKey(226), getKey(227), getKey(228), getKey(229), getKey(230),
        getKey(231), getKey(232), getKey(233), getKey(234), getKey(235), getKey(236), getKey(237),
        getKey(238), getKey(239), getKey(240), getKey(241), getKey(242), getKey(243), getKey(244),
        getKey(245), getKey(246), getKey(247), getKey(248), getKey(249), getKey(250), getKey(251),
        getKey(252), getKey(253), getKey(254), getKey(255),
    };


    inline u64 getModifierBits(const int vk_code, const int bit_index, u64* keyDown) {
        u64 bit = 0;

        if (GetAsyncKeyState(vk_code) & 0x80'00) bit |= 1ull << bit_index;
        if (GetAsyncKeyState(vk_code + 1) & 0x80'00) bit |= 1ull << (bit_index + 1);

        if (!(GetKeyState(vk_code) & 0x80'00) && *keyDown & 1ull << bit_index)
            *keyDown &= ~(1ull << bit_index);
        if (!(GetKeyState(vk_code + 1) & 0x80'00) && *keyDown & 1ull << (bit_index + 1))
            *keyDown &= ~(1ull << (bit_index + 1));

        return bit;
    }
}

#endif
#endif
