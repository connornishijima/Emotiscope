#define LED_DATA_1_PIN ( 21 )
#define LED_DATA_2_PIN ( 17 )

// 32-bit color input
extern CRGBF leds[ NUM_LEDS ];

// Remapped to floating 8-bit range
extern CRGBF leds_scaled[ NUM_LEDS ];

CRGBF dither_error[ NUM_LEDS ];

// True 8-bit color output
static uint8_t raw_led_data[ NUM_LEDS*3 ];

float gamma_correction_lookup[2048] = {
	0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 
	0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0001, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0002, 0.0003, 0.0003, 0.0003, 0.0003, 0.0003, 0.0003, 0.0003, 0.0003, 0.0003, 0.0003, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0004, 0.0005, 0.0005, 0.0005, 0.0005, 0.0005, 
	0.0005, 0.0005, 0.0005, 0.0006, 0.0006, 0.0006, 0.0006, 0.0006, 0.0006, 0.0006, 0.0007, 0.0007, 0.0007, 0.0007, 0.0007, 0.0007, 0.0007, 0.0008, 0.0008, 0.0008, 0.0008, 0.0008, 0.0008, 0.0009, 0.0009, 0.0009, 0.0009, 0.0009, 0.0010, 0.0010, 0.0010, 0.0010, 0.0010, 0.0011, 0.0011, 0.0011, 0.0011, 0.0011, 0.0012, 0.0012, 0.0012, 0.0012, 0.0012, 0.0013, 0.0013, 0.0013, 0.0013, 0.0014, 0.0014, 0.0014, 0.0014, 0.0015, 0.0015, 0.0015, 0.0015, 
	0.0016, 0.0016, 0.0016, 0.0016, 0.0017, 0.0017, 0.0017, 0.0017, 0.0018, 0.0018, 0.0018, 0.0018, 0.0019, 0.0019, 0.0019, 0.0020, 0.0020, 0.0020, 0.0020, 0.0021, 0.0021, 0.0021, 0.0022, 0.0022, 0.0022, 0.0023, 0.0023, 0.0023, 0.0024, 0.0024, 0.0024, 0.0025, 0.0025, 0.0025, 0.0026, 0.0026, 0.0026, 0.0027, 0.0027, 0.0027, 0.0028, 0.0028, 0.0028, 0.0029, 0.0029, 0.0029, 0.0030, 0.0030, 0.0031, 0.0031, 0.0031, 0.0032, 0.0032, 0.0033, 0.0033, 
	0.0033, 0.0034, 0.0034, 0.0035, 0.0035, 0.0035, 0.0036, 0.0036, 0.0037, 0.0037, 0.0037, 0.0038, 0.0038, 0.0039, 0.0039, 0.0040, 0.0040, 0.0041, 0.0041, 0.0041, 0.0042, 0.0042, 0.0043, 0.0043, 0.0044, 0.0044, 0.0045, 0.0045, 0.0046, 0.0046, 0.0047, 0.0047, 0.0048, 0.0048, 0.0049, 0.0049, 0.0050, 0.0050, 0.0051, 0.0051, 0.0052, 0.0052, 0.0053, 0.0053, 0.0054, 0.0054, 0.0055, 0.0055, 0.0056, 0.0056, 0.0057, 0.0057, 0.0058, 0.0059, 0.0059, 
	0.0060, 0.0060, 0.0061, 0.0061, 0.0062, 0.0063, 0.0063, 0.0064, 0.0064, 0.0065, 0.0066, 0.0066, 0.0067, 0.0067, 0.0068, 0.0069, 0.0069, 0.0070, 0.0070, 0.0071, 0.0072, 0.0072, 0.0073, 0.0074, 0.0074, 0.0075, 0.0076, 0.0076, 0.0077, 0.0078, 0.0078, 0.0079, 0.0080, 0.0080, 0.0081, 0.0082, 0.0082, 0.0083, 0.0084, 0.0084, 0.0085, 0.0086, 0.0086, 0.0087, 0.0088, 0.0089, 0.0089, 0.0090, 0.0091, 0.0091, 0.0092, 0.0093, 0.0094, 0.0094, 0.0095, 
	0.0096, 0.0097, 0.0097, 0.0098, 0.0099, 0.0100, 0.0100, 0.0101, 0.0102, 0.0103, 0.0104, 0.0104, 0.0105, 0.0106, 0.0107, 0.0108, 0.0108, 0.0109, 0.0110, 0.0111, 0.0112, 0.0112, 0.0113, 0.0114, 0.0115, 0.0116, 0.0117, 0.0117, 0.0118, 0.0119, 0.0120, 0.0121, 0.0122, 0.0123, 0.0123, 0.0124, 0.0125, 0.0126, 0.0127, 0.0128, 0.0129, 0.0130, 0.0131, 0.0132, 0.0132, 0.0133, 0.0134, 0.0135, 0.0136, 0.0137, 0.0138, 0.0139, 0.0140, 0.0141, 0.0142, 
	0.0143, 0.0144, 0.0145, 0.0146, 0.0147, 0.0148, 0.0148, 0.0149, 0.0150, 0.0151, 0.0152, 0.0153, 0.0154, 0.0155, 0.0156, 0.0157, 0.0158, 0.0159, 0.0160, 0.0162, 0.0163, 0.0164, 0.0165, 0.0166, 0.0167, 0.0168, 0.0169, 0.0170, 0.0171, 0.0172, 0.0173, 0.0174, 0.0175, 0.0176, 0.0177, 0.0178, 0.0180, 0.0181, 0.0182, 0.0183, 0.0184, 0.0185, 0.0186, 0.0187, 0.0188, 0.0190, 0.0191, 0.0192, 0.0193, 0.0194, 0.0195, 0.0196, 0.0198, 0.0199, 0.0200, 
	0.0201, 0.0202, 0.0203, 0.0205, 0.0206, 0.0207, 0.0208, 0.0209, 0.0211, 0.0212, 0.0213, 0.0214, 0.0215, 0.0217, 0.0218, 0.0219, 0.0220, 0.0222, 0.0223, 0.0224, 0.0225, 0.0227, 0.0228, 0.0229, 0.0230, 0.0232, 0.0233, 0.0234, 0.0236, 0.0237, 0.0238, 0.0239, 0.0241, 0.0242, 0.0243, 0.0245, 0.0246, 0.0247, 0.0249, 0.0250, 0.0251, 0.0253, 0.0254, 0.0255, 0.0257, 0.0258, 0.0259, 0.0261, 0.0262, 0.0263, 0.0265, 0.0266, 0.0268, 0.0269, 0.0270, 
	0.0272, 0.0273, 0.0275, 0.0276, 0.0277, 0.0279, 0.0280, 0.0282, 0.0283, 0.0285, 0.0286, 0.0288, 0.0289, 0.0290, 0.0292, 0.0293, 0.0295, 0.0296, 0.0298, 0.0299, 0.0301, 0.0302, 0.0304, 0.0305, 0.0307, 0.0308, 0.0310, 0.0311, 0.0313, 0.0314, 0.0316, 0.0317, 0.0319, 0.0321, 0.0322, 0.0324, 0.0325, 0.0327, 0.0328, 0.0330, 0.0332, 0.0333, 0.0335, 0.0336, 0.0338, 0.0340, 0.0341, 0.0343, 0.0344, 0.0346, 0.0348, 0.0349, 0.0351, 0.0352, 0.0354, 
	0.0356, 0.0357, 0.0359, 0.0361, 0.0362, 0.0364, 0.0366, 0.0367, 0.0369, 0.0371, 0.0373, 0.0374, 0.0376, 0.0378, 0.0379, 0.0381, 0.0383, 0.0384, 0.0386, 0.0388, 0.0390, 0.0391, 0.0393, 0.0395, 0.0397, 0.0398, 0.0400, 0.0402, 0.0404, 0.0406, 0.0407, 0.0409, 0.0411, 0.0413, 0.0415, 0.0416, 0.0418, 0.0420, 0.0422, 0.0424, 0.0426, 0.0427, 0.0429, 0.0431, 0.0433, 0.0435, 0.0437, 0.0438, 0.0440, 0.0442, 0.0444, 0.0446, 0.0448, 0.0450, 0.0452, 
	0.0454, 0.0456, 0.0457, 0.0459, 0.0461, 0.0463, 0.0465, 0.0467, 0.0469, 0.0471, 0.0473, 0.0475, 0.0477, 0.0479, 0.0481, 0.0483, 0.0485, 0.0487, 0.0489, 0.0491, 0.0493, 0.0495, 0.0497, 0.0499, 0.0501, 0.0503, 0.0505, 0.0507, 0.0509, 0.0511, 0.0513, 0.0515, 0.0517, 0.0519, 0.0521, 0.0523, 0.0525, 0.0528, 0.0530, 0.0532, 0.0534, 0.0536, 0.0538, 0.0540, 0.0542, 0.0544, 0.0547, 0.0549, 0.0551, 0.0553, 0.0555, 0.0557, 0.0559, 0.0562, 0.0564, 
	0.0566, 0.0568, 0.0570, 0.0573, 0.0575, 0.0577, 0.0579, 0.0581, 0.0584, 0.0586, 0.0588, 0.0590, 0.0593, 0.0595, 0.0597, 0.0599, 0.0602, 0.0604, 0.0606, 0.0608, 0.0611, 0.0613, 0.0615, 0.0617, 0.0620, 0.0622, 0.0624, 0.0627, 0.0629, 0.0631, 0.0634, 0.0636, 0.0638, 0.0641, 0.0643, 0.0645, 0.0648, 0.0650, 0.0653, 0.0655, 0.0657, 0.0660, 0.0662, 0.0664, 0.0667, 0.0669, 0.0672, 0.0674, 0.0677, 0.0679, 0.0681, 0.0684, 0.0686, 0.0689, 0.0691, 
	0.0694, 0.0696, 0.0699, 0.0701, 0.0704, 0.0706, 0.0709, 0.0711, 0.0714, 0.0716, 0.0719, 0.0721, 0.0724, 0.0726, 0.0729, 0.0731, 0.0734, 0.0736, 0.0739, 0.0741, 0.0744, 0.0747, 0.0749, 0.0752, 0.0754, 0.0757, 0.0759, 0.0762, 0.0765, 0.0767, 0.0770, 0.0773, 0.0775, 0.0778, 0.0780, 0.0783, 0.0786, 0.0788, 0.0791, 0.0794, 0.0796, 0.0799, 0.0802, 0.0804, 0.0807, 0.0810, 0.0813, 0.0815, 0.0818, 0.0821, 0.0823, 0.0826, 0.0829, 0.0832, 0.0834, 
	0.0837, 0.0840, 0.0843, 0.0845, 0.0848, 0.0851, 0.0854, 0.0857, 0.0859, 0.0862, 0.0865, 0.0868, 0.0871, 0.0873, 0.0876, 0.0879, 0.0882, 0.0885, 0.0888, 0.0891, 0.0893, 0.0896, 0.0899, 0.0902, 0.0905, 0.0908, 0.0911, 0.0914, 0.0917, 0.0919, 0.0922, 0.0925, 0.0928, 0.0931, 0.0934, 0.0937, 0.0940, 0.0943, 0.0946, 0.0949, 0.0952, 0.0955, 0.0958, 0.0961, 0.0964, 0.0967, 0.0970, 0.0973, 0.0976, 0.0979, 0.0982, 0.0985, 0.0988, 0.0991, 0.0994, 
	0.0997, 0.1000, 0.1003, 0.1006, 0.1009, 0.1013, 0.1016, 0.1019, 0.1022, 0.1025, 0.1028, 0.1031, 0.1034, 0.1037, 0.1041, 0.1044, 0.1047, 0.1050, 0.1053, 0.1056, 0.1060, 0.1063, 0.1066, 0.1069, 0.1072, 0.1075, 0.1079, 0.1082, 0.1085, 0.1088, 0.1092, 0.1095, 0.1098, 0.1101, 0.1105, 0.1108, 0.1111, 0.1114, 0.1118, 0.1121, 0.1124, 0.1128, 0.1131, 0.1134, 0.1137, 0.1141, 0.1144, 0.1147, 0.1151, 0.1154, 0.1157, 0.1161, 0.1164, 0.1167, 0.1171, 
	0.1174, 0.1178, 0.1181, 0.1184, 0.1188, 0.1191, 0.1195, 0.1198, 0.1201, 0.1205, 0.1208, 0.1212, 0.1215, 0.1219, 0.1222, 0.1226, 0.1229, 0.1233, 0.1236, 0.1239, 0.1243, 0.1246, 0.1250, 0.1253, 0.1257, 0.1261, 0.1264, 0.1268, 0.1271, 0.1275, 0.1278, 0.1282, 0.1285, 0.1289, 0.1292, 0.1296, 0.1300, 0.1303, 0.1307, 0.1310, 0.1314, 0.1318, 0.1321, 0.1325, 0.1329, 0.1332, 0.1336, 0.1340, 0.1343, 0.1347, 0.1350, 0.1354, 0.1358, 0.1362, 0.1365, 
	0.1369, 0.1373, 0.1376, 0.1380, 0.1384, 0.1388, 0.1391, 0.1395, 0.1399, 0.1403, 0.1406, 0.1410, 0.1414, 0.1418, 0.1421, 0.1425, 0.1429, 0.1433, 0.1437, 0.1440, 0.1444, 0.1448, 0.1452, 0.1456, 0.1460, 0.1463, 0.1467, 0.1471, 0.1475, 0.1479, 0.1483, 0.1487, 0.1491, 0.1494, 0.1498, 0.1502, 0.1506, 0.1510, 0.1514, 0.1518, 0.1522, 0.1526, 0.1530, 0.1534, 0.1538, 0.1542, 0.1546, 0.1550, 0.1554, 0.1558, 0.1562, 0.1566, 0.1570, 0.1574, 0.1578, 
	0.1582, 0.1586, 0.1590, 0.1594, 0.1598, 0.1602, 0.1606, 0.1610, 0.1614, 0.1618, 0.1623, 0.1627, 0.1631, 0.1635, 0.1639, 0.1643, 0.1647, 0.1651, 0.1656, 0.1660, 0.1664, 0.1668, 0.1672, 0.1676, 0.1681, 0.1685, 0.1689, 0.1693, 0.1697, 0.1702, 0.1706, 0.1710, 0.1714, 0.1719, 0.1723, 0.1727, 0.1731, 0.1736, 0.1740, 0.1744, 0.1748, 0.1753, 0.1757, 0.1761, 0.1766, 0.1770, 0.1774, 0.1779, 0.1783, 0.1787, 0.1792, 0.1796, 0.1800, 0.1805, 0.1809, 
	0.1813, 0.1818, 0.1822, 0.1827, 0.1831, 0.1835, 0.1840, 0.1844, 0.1849, 0.1853, 0.1858, 0.1862, 0.1867, 0.1871, 0.1875, 0.1880, 0.1884, 0.1889, 0.1893, 0.1898, 0.1902, 0.1907, 0.1911, 0.1916, 0.1921, 0.1925, 0.1930, 0.1934, 0.1939, 0.1943, 0.1948, 0.1952, 0.1957, 0.1962, 0.1966, 0.1971, 0.1975, 0.1980, 0.1985, 0.1989, 0.1994, 0.1999, 0.2003, 0.2008, 0.2013, 0.2017, 0.2022, 0.2027, 0.2031, 0.2036, 0.2041, 0.2045, 0.2050, 0.2055, 0.2060, 
	0.2064, 0.2069, 0.2074, 0.2079, 0.2083, 0.2088, 0.2093, 0.2098, 0.2102, 0.2107, 0.2112, 0.2117, 0.2122, 0.2126, 0.2131, 0.2136, 0.2141, 0.2146, 0.2151, 0.2156, 0.2160, 0.2165, 0.2170, 0.2175, 0.2180, 0.2185, 0.2190, 0.2195, 0.2200, 0.2204, 0.2209, 0.2214, 0.2219, 0.2224, 0.2229, 0.2234, 0.2239, 0.2244, 0.2249, 0.2254, 0.2259, 0.2264, 0.2269, 0.2274, 0.2279, 0.2284, 0.2289, 0.2294, 0.2299, 0.2304, 0.2309, 0.2315, 0.2320, 0.2325, 0.2330, 
	0.2335, 0.2340, 0.2345, 0.2350, 0.2355, 0.2361, 0.2366, 0.2371, 0.2376, 0.2381, 0.2386, 0.2391, 0.2397, 0.2402, 0.2407, 0.2412, 0.2417, 0.2423, 0.2428, 0.2433, 0.2438, 0.2444, 0.2449, 0.2454, 0.2459, 0.2465, 0.2470, 0.2475, 0.2480, 0.2486, 0.2491, 0.2496, 0.2502, 0.2507, 0.2512, 0.2518, 0.2523, 0.2528, 0.2534, 0.2539, 0.2544, 0.2550, 0.2555, 0.2561, 0.2566, 0.2571, 0.2577, 0.2582, 0.2588, 0.2593, 0.2598, 0.2604, 0.2609, 0.2615, 0.2620, 
	0.2626, 0.2631, 0.2637, 0.2642, 0.2648, 0.2653, 0.2659, 0.2664, 0.2670, 0.2675, 0.2681, 0.2686, 0.2692, 0.2698, 0.2703, 0.2709, 0.2714, 0.2720, 0.2725, 0.2731, 0.2737, 0.2742, 0.2748, 0.2753, 0.2759, 0.2765, 0.2770, 0.2776, 0.2782, 0.2787, 0.2793, 0.2799, 0.2804, 0.2810, 0.2816, 0.2822, 0.2827, 0.2833, 0.2839, 0.2845, 0.2850, 0.2856, 0.2862, 0.2868, 0.2873, 0.2879, 0.2885, 0.2891, 0.2896, 0.2902, 0.2908, 0.2914, 0.2920, 0.2926, 0.2931, 
	0.2937, 0.2943, 0.2949, 0.2955, 0.2961, 0.2967, 0.2973, 0.2978, 0.2984, 0.2990, 0.2996, 0.3002, 0.3008, 0.3014, 0.3020, 0.3026, 0.3032, 0.3038, 0.3044, 0.3050, 0.3056, 0.3062, 0.3068, 0.3074, 0.3080, 0.3086, 0.3092, 0.3098, 0.3104, 0.3110, 0.3116, 0.3122, 0.3128, 0.3134, 0.3140, 0.3147, 0.3153, 0.3159, 0.3165, 0.3171, 0.3177, 0.3183, 0.3189, 0.3196, 0.3202, 0.3208, 0.3214, 0.3220, 0.3227, 0.3233, 0.3239, 0.3245, 0.3251, 0.3258, 0.3264, 
	0.3270, 0.3276, 0.3283, 0.3289, 0.3295, 0.3301, 0.3308, 0.3314, 0.3320, 0.3327, 0.3333, 0.3339, 0.3346, 0.3352, 0.3358, 0.3365, 0.3371, 0.3377, 0.3384, 0.3390, 0.3396, 0.3403, 0.3409, 0.3416, 0.3422, 0.3428, 0.3435, 0.3441, 0.3448, 0.3454, 0.3461, 0.3467, 0.3474, 0.3480, 0.3487, 0.3493, 0.3500, 0.3506, 0.3513, 0.3519, 0.3526, 0.3532, 0.3539, 0.3545, 0.3552, 0.3558, 0.3565, 0.3572, 0.3578, 0.3585, 0.3591, 0.3598, 0.3605, 0.3611, 0.3618, 
	0.3624, 0.3631, 0.3638, 0.3644, 0.3651, 0.3658, 0.3664, 0.3671, 0.3678, 0.3685, 0.3691, 0.3698, 0.3705, 0.3711, 0.3718, 0.3725, 0.3732, 0.3738, 0.3745, 0.3752, 0.3759, 0.3766, 0.3772, 0.3779, 0.3786, 0.3793, 0.3800, 0.3806, 0.3813, 0.3820, 0.3827, 0.3834, 0.3841, 0.3848, 0.3855, 0.3861, 0.3868, 0.3875, 0.3882, 0.3889, 0.3896, 0.3903, 0.3910, 0.3917, 0.3924, 0.3931, 0.3938, 0.3945, 0.3952, 0.3959, 0.3966, 0.3973, 0.3980, 0.3987, 0.3994, 
	0.4001, 0.4008, 0.4015, 0.4022, 0.4029, 0.4036, 0.4043, 0.4050, 0.4058, 0.4065, 0.4072, 0.4079, 0.4086, 0.4093, 0.4100, 0.4108, 0.4115, 0.4122, 0.4129, 0.4136, 0.4143, 0.4151, 0.4158, 0.4165, 0.4172, 0.4179, 0.4187, 0.4194, 0.4201, 0.4208, 0.4216, 0.4223, 0.4230, 0.4238, 0.4245, 0.4252, 0.4260, 0.4267, 0.4274, 0.4282, 0.4289, 0.4296, 0.4304, 0.4311, 0.4318, 0.4326, 0.4333, 0.4341, 0.4348, 0.4355, 0.4363, 0.4370, 0.4378, 0.4385, 0.4393, 
	0.4400, 0.4407, 0.4415, 0.4422, 0.4430, 0.4437, 0.4445, 0.4452, 0.4460, 0.4467, 0.4475, 0.4483, 0.4490, 0.4498, 0.4505, 0.4513, 0.4520, 0.4528, 0.4536, 0.4543, 0.4551, 0.4558, 0.4566, 0.4574, 0.4581, 0.4589, 0.4597, 0.4604, 0.4612, 0.4620, 0.4627, 0.4635, 0.4643, 0.4650, 0.4658, 0.4666, 0.4674, 0.4681, 0.4689, 0.4697, 0.4705, 0.4712, 0.4720, 0.4728, 0.4736, 0.4744, 0.4751, 0.4759, 0.4767, 0.4775, 0.4783, 0.4790, 0.4798, 0.4806, 0.4814, 
	0.4822, 0.4830, 0.4838, 0.4846, 0.4854, 0.4861, 0.4869, 0.4877, 0.4885, 0.4893, 0.4901, 0.4909, 0.4917, 0.4925, 0.4933, 0.4941, 0.4949, 0.4957, 0.4965, 0.4973, 0.4981, 0.4989, 0.4997, 0.5005, 0.5013, 0.5021, 0.5030, 0.5038, 0.5046, 0.5054, 0.5062, 0.5070, 0.5078, 0.5086, 0.5095, 0.5103, 0.5111, 0.5119, 0.5127, 0.5135, 0.5144, 0.5152, 0.5160, 0.5168, 0.5176, 0.5185, 0.5193, 0.5201, 0.5209, 0.5218, 0.5226, 0.5234, 0.5242, 0.5251, 0.5259, 
	0.5267, 0.5276, 0.5284, 0.5292, 0.5301, 0.5309, 0.5317, 0.5326, 0.5334, 0.5343, 0.5351, 0.5359, 0.5368, 0.5376, 0.5385, 0.5393, 0.5401, 0.5410, 0.5418, 0.5427, 0.5435, 0.5444, 0.5452, 0.5461, 0.5469, 0.5478, 0.5486, 0.5495, 0.5503, 0.5512, 0.5520, 0.5529, 0.5537, 0.5546, 0.5555, 0.5563, 0.5572, 0.5580, 0.5589, 0.5598, 0.5606, 0.5615, 0.5624, 0.5632, 0.5641, 0.5649, 0.5658, 0.5667, 0.5676, 0.5684, 0.5693, 0.5702, 0.5710, 0.5719, 0.5728, 
	0.5737, 0.5745, 0.5754, 0.5763, 0.5772, 0.5780, 0.5789, 0.5798, 0.5807, 0.5816, 0.5824, 0.5833, 0.5842, 0.5851, 0.5860, 0.5869, 0.5878, 0.5886, 0.5895, 0.5904, 0.5913, 0.5922, 0.5931, 0.5940, 0.5949, 0.5958, 0.5967, 0.5976, 0.5985, 0.5994, 0.6003, 0.6012, 0.6021, 0.6030, 0.6039, 0.6048, 0.6057, 0.6066, 0.6075, 0.6084, 0.6093, 0.6102, 0.6111, 0.6120, 0.6129, 0.6138, 0.6148, 0.6157, 0.6166, 0.6175, 0.6184, 0.6193, 0.6202, 0.6212, 0.6221, 
	0.6230, 0.6239, 0.6248, 0.6258, 0.6267, 0.6276, 0.6285, 0.6295, 0.6304, 0.6313, 0.6322, 0.6332, 0.6341, 0.6350, 0.6360, 0.6369, 0.6378, 0.6387, 0.6397, 0.6406, 0.6416, 0.6425, 0.6434, 0.6444, 0.6453, 0.6462, 0.6472, 0.6481, 0.6491, 0.6500, 0.6510, 0.6519, 0.6528, 0.6538, 0.6547, 0.6557, 0.6566, 0.6576, 0.6585, 0.6595, 0.6604, 0.6614, 0.6623, 0.6633, 0.6642, 0.6652, 0.6662, 0.6671, 0.6681, 0.6690, 0.6700, 0.6710, 0.6719, 0.6729, 0.6738, 
	0.6748, 0.6758, 0.6767, 0.6777, 0.6787, 0.6796, 0.6806, 0.6816, 0.6826, 0.6835, 0.6845, 0.6855, 0.6864, 0.6874, 0.6884, 0.6894, 0.6903, 0.6913, 0.6923, 0.6933, 0.6943, 0.6952, 0.6962, 0.6972, 0.6982, 0.6992, 0.7002, 0.7012, 0.7021, 0.7031, 0.7041, 0.7051, 0.7061, 0.7071, 0.7081, 0.7091, 0.7101, 0.7111, 0.7121, 0.7131, 0.7141, 0.7151, 0.7161, 0.7171, 0.7181, 0.7191, 0.7201, 0.7211, 0.7221, 0.7231, 0.7241, 0.7251, 0.7261, 0.7271, 0.7281, 
	0.7291, 0.7301, 0.7311, 0.7322, 0.7332, 0.7342, 0.7352, 0.7362, 0.7372, 0.7382, 0.7393, 0.7403, 0.7413, 0.7423, 0.7433, 0.7444, 0.7454, 0.7464, 0.7474, 0.7485, 0.7495, 0.7505, 0.7515, 0.7526, 0.7536, 0.7546, 0.7557, 0.7567, 0.7577, 0.7588, 0.7598, 0.7608, 0.7619, 0.7629, 0.7640, 0.7650, 0.7660, 0.7671, 0.7681, 0.7692, 0.7702, 0.7713, 0.7723, 0.7733, 0.7744, 0.7754, 0.7765, 0.7775, 0.7786, 0.7796, 0.7807, 0.7817, 0.7828, 0.7839, 0.7849, 
	0.7860, 0.7870, 0.7881, 0.7891, 0.7902, 0.7913, 0.7923, 0.7934, 0.7944, 0.7955, 0.7966, 0.7976, 0.7987, 0.7998, 0.8008, 0.8019, 0.8030, 0.8041, 0.8051, 0.8062, 0.8073, 0.8084, 0.8094, 0.8105, 0.8116, 0.8127, 0.8137, 0.8148, 0.8159, 0.8170, 0.8181, 0.8191, 0.8202, 0.8213, 0.8224, 0.8235, 0.8246, 0.8257, 0.8267, 0.8278, 0.8289, 0.8300, 0.8311, 0.8322, 0.8333, 0.8344, 0.8355, 0.8366, 0.8377, 0.8388, 0.8399, 0.8410, 0.8421, 0.8432, 0.8443, 0.8454, 0.8465, 0.8476, 0.8487, 0.8498, 
	0.8509, 0.8520, 0.8531, 0.8543, 0.8554, 0.8565, 0.8576, 0.8587, 0.8598, 0.8609, 0.8621, 0.8632, 0.8643, 0.8654, 0.8665, 0.8677, 0.8688, 0.8699, 0.8710, 0.8721, 0.8733, 0.8744, 0.8755, 0.8767, 0.8778, 0.8789, 0.8800, 0.8812, 0.8823, 0.8834, 0.8846, 0.8857, 0.8868, 0.8880, 0.8891, 0.8903, 0.8914, 0.8925, 0.8937, 0.8948, 0.8960, 0.8971, 0.8983, 0.8994, 0.9005, 0.9017, 0.9028, 0.9040, 0.9051, 0.9063, 0.9074, 0.9086, 0.9097, 0.9109, 0.9121, 0.9132, 0.9144, 0.9155, 0.9167, 0.9178, 0.9190, 0.9202, 0.9213, 0.9225, 0.9237, 0.9248, 0.9260, 0.9272, 0.9283, 0.9295, 0.9307, 0.9318, 0.9330, 0.9342, 0.9353, 
	0.9365, 0.9377, 0.9389, 0.9400, 0.9412, 0.9424, 0.9436, 0.9448, 0.9459, 0.9471, 0.9483, 0.9495, 0.9507, 0.9519, 0.9530, 0.9542, 0.9554, 0.9566, 0.9578, 0.9590, 0.9602, 0.9614, 0.9626, 0.9638, 0.9650, 0.9662, 0.9674, 0.9685, 0.9697, 0.9709, 0.9721, 0.9733, 0.9745, 0.9758, 0.9770, 0.9782, 0.9794, 0.9806, 0.9818, 0.9830, 0.9842, 0.9854, 0.9866, 0.9878, 0.9890, 0.9903, 0.9915, 0.9927, 0.9939, 0.9951, 0.9963, 0.9976, 0.9988, 1.0000
};

rmt_channel_handle_t tx_chan_a = NULL;
rmt_channel_handle_t tx_chan_b = NULL;
rmt_encoder_handle_t led_encoder_a = NULL;
rmt_encoder_handle_t led_encoder_b = NULL;

uint32_t lfsr = 0xACE1u;  // Initial seed for LFSR
const uint32_t polynomial = 0x10000000u;  // Polynomial for LFSR

typedef struct {
    rmt_encoder_t		base;
    rmt_encoder_t		*bytes_encoder;
    rmt_encoder_t		*copy_encoder;
    int 				state;
    rmt_symbol_word_t	reset_code;
} rmt_led_strip_encoder_t;

rmt_led_strip_encoder_t strip_encoder_a;
rmt_led_strip_encoder_t strip_encoder_b;

rmt_transmit_config_t tx_config = {
	.loop_count = 0,  // no transfer loop
	.flags      = {
		.eot_level         = 0,
		.queue_nonblocking = 0,
	},
};

typedef struct {
    uint32_t resolution; /* Encoder resolution, in Hz */
} led_strip_encoder_config_t;

void init_random_dither_error(){
	for(uint16_t i = 0; i < NUM_LEDS; i++){
		dither_error[ i ].r = esp_random() / (float)UINT32_MAX;
		dither_error[ i ].g = esp_random() / (float)UINT32_MAX;
		dither_error[ i ].b = esp_random() / (float)UINT32_MAX;
	}
}

IRAM_ATTR static size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state){
    rmt_led_strip_encoder_t *led_encoder = __containerof( encoder, rmt_led_strip_encoder_t, base );
    rmt_encoder_handle_t   bytes_encoder = led_encoder->bytes_encoder;
    rmt_encoder_handle_t    copy_encoder = led_encoder->copy_encoder;
    rmt_encode_state_t     session_state = RMT_ENCODING_RESET;
    rmt_encode_state_t             state = RMT_ENCODING_RESET;

    size_t encoded_symbols = 0;

    switch ( led_encoder->state ) {
		case 0: // send RGB data
			encoded_symbols += bytes_encoder->encode(
				bytes_encoder,
				channel,
				primary_data,
				data_size,
				&session_state
			);

			if ( session_state & RMT_ENCODING_COMPLETE ) {
				led_encoder->state = 1; // switch to next state when current encoding session finished
			}
			if ( session_state & RMT_ENCODING_MEM_FULL ) {
				state = (rmt_encode_state_t)( state | (uint32_t)RMT_ENCODING_MEM_FULL );
				goto out; // yield if there's no free space for encoding artifacts
			}
		// fall-through
		case 1: // send reset code
			encoded_symbols += copy_encoder->encode(
				copy_encoder,
				channel,
				&led_encoder->reset_code,
				sizeof( led_encoder->reset_code ),
				&session_state
			);

			if (session_state & RMT_ENCODING_COMPLETE) {
				led_encoder->state = RMT_ENCODING_RESET; // back to the initial encoding session
				state = (rmt_encode_state_t)(state | (uint32_t)RMT_ENCODING_COMPLETE);
			}
			if (session_state & RMT_ENCODING_MEM_FULL) {
				state = (rmt_encode_state_t)(state | (uint32_t)RMT_ENCODING_MEM_FULL);
				goto out; // yield if there's no free space for encoding artifacts
			}
	}
out:
    *ret_state = state;
    return encoded_symbols;
}

static esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder){
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_del_encoder(led_encoder->bytes_encoder);
    rmt_del_encoder(led_encoder->copy_encoder);
    free(led_encoder);
    return ESP_OK;
}

static esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder){
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_reset(led_encoder->bytes_encoder);
    rmt_encoder_reset(led_encoder->copy_encoder);
    led_encoder->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

esp_err_t rmt_new_led_strip_encoder(const led_strip_encoder_config_t *config, rmt_encoder_handle_t *ret_encoder_a, rmt_encoder_handle_t *ret_encoder_b){
	strip_encoder_a.base.encode = rmt_encode_led_strip;
    strip_encoder_a.base.del    = rmt_del_led_strip_encoder;
    strip_encoder_a.base.reset  = rmt_led_strip_encoder_reset;

	strip_encoder_b.base.encode = rmt_encode_led_strip;
    strip_encoder_b.base.del    = rmt_del_led_strip_encoder;
    strip_encoder_b.base.reset  = rmt_led_strip_encoder_reset;

    // different led strip might have its own timing requirements, following parameter is for WS2812
    rmt_bytes_encoder_config_t bytes_encoder_config = {
        .bit0 = { { 4, 1, 6, 0 } },
        .bit1 = { { 7, 1, 6, 0 } },
		.flags = { .msb_first = 1 }
    };
    
	rmt_new_bytes_encoder(&bytes_encoder_config, &strip_encoder_a.bytes_encoder);
	rmt_new_bytes_encoder(&bytes_encoder_config, &strip_encoder_b.bytes_encoder);
    rmt_copy_encoder_config_t copy_encoder_config = {};
    rmt_new_copy_encoder(&copy_encoder_config, &strip_encoder_a.copy_encoder);
	rmt_new_copy_encoder(&copy_encoder_config, &strip_encoder_b.copy_encoder);

    strip_encoder_a.reset_code = (rmt_symbol_word_t) { { 250, 0, 250, 0 } };
    strip_encoder_b.reset_code = (rmt_symbol_word_t) { { 250, 0, 250, 0 } };

    *ret_encoder_a = &strip_encoder_a.base;
    *ret_encoder_b = &strip_encoder_b.base;
    return ESP_OK;
}

void init_rmt_driver() {
	printf("init_rmt_driver\n");
	rmt_tx_channel_config_t tx_chan_a_config = {
		.gpio_num = (gpio_num_t)LED_DATA_1_PIN,	// GPIO number
		.clk_src = RMT_CLK_SRC_DEFAULT,	 // select source clock
		.resolution_hz = 10000000,		 // 10 MHz tick resolution, i.e., 1 tick = 0.1 µs
		.mem_block_symbols = 128,		 // memory block size, 64 * 4 = 256 Bytes
		.trans_queue_depth = 4,			 // set the number of transactions that can be pending in the background
		.intr_priority = 99,
		.flags = {
			.invert_out   = 0,
			.with_dma     = 0,
			.io_loop_back = 0,
        	.io_od_mode   = 0
		},
	};

	rmt_tx_channel_config_t tx_chan_b_config = {
		.gpio_num = (gpio_num_t)LED_DATA_2_PIN, // GPIO number
		.clk_src = RMT_CLK_SRC_DEFAULT,	 // select source clock
		.resolution_hz = 10000000,		 // 10 MHz tick resolution, i.e., 1 tick = 0.1 µs
		.mem_block_symbols = 128,		 // memory block size, 64 * 4 = 256 Bytes
		.trans_queue_depth = 4,			 // set the number of transactions that can be pending in the background
		.intr_priority = 99,
		.flags = {
			.invert_out   = 0,
			.with_dma     = 0,
			.io_loop_back = 0,
        	.io_od_mode   = 0
		},
	};

	printf("rmt_new_tx_channel\n");
	ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_a_config, &tx_chan_a));
	ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_b_config, &tx_chan_b));

	ESP_LOGI(TAG, "Install led strip encoder");
    led_strip_encoder_config_t encoder_config = {
        .resolution = 10000000,
    };
	printf("rmt_new_led_strip_encoder\n");
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder_a, &led_encoder_b));
	
	printf("rmt_enable\n");
	ESP_ERROR_CHECK(rmt_enable(tx_chan_a));
	ESP_ERROR_CHECK(rmt_enable(tx_chan_b));

	init_random_dither_error();
}

void quantize_color_error(bool temporal_dithering){
	memcpy(leds_scaled, leds, NUM_LEDS * sizeof(CRGBF));
	dsps_mulc_f32_ansi((float*)leds, (float*)leds_scaled, NUM_LEDS*3, 255.0, 1, 1);

	if(temporal_dithering == true){
		for (uint16_t i = 0; i < NUM_LEDS; i++) {
			raw_led_data[3*i+1] = (uint8_t)(leds_scaled[i].r);
			raw_led_data[3*i+0] = (uint8_t)(leds_scaled[i].g);
			raw_led_data[3*i+2] = (uint8_t)(leds_scaled[i].b);

			float new_error_r = leds_scaled[i].r - raw_led_data[3*i+1];
			float new_error_g = leds_scaled[i].g - raw_led_data[3*i+0];
			float new_error_b = leds_scaled[i].b - raw_led_data[3*i+2];

			const float dither_error_threshold = (0.055)/2.0;
			if(new_error_r >= dither_error_threshold){ dither_error[i].r += new_error_r; }
			if(new_error_g >= dither_error_threshold){ dither_error[i].g += new_error_g; }
			if(new_error_b >= dither_error_threshold){ dither_error[i].b += new_error_b; }

			if(dither_error[i].r >= 1.0){ raw_led_data[3*i+1] += 1; dither_error[i].r -= 1.0; }
			if(dither_error[i].g >= 1.0){ raw_led_data[3*i+0] += 1; dither_error[i].g -= 1.0; }
			if(dither_error[i].b >= 1.0){ raw_led_data[3*i+2] += 1; dither_error[i].b -= 1.0; }
		}
	}
	else{
		for (uint16_t i = 0; i < NUM_LEDS; i++) {
			raw_led_data[3*i+1] = round(leds_scaled[i].r);
			raw_led_data[3*i+0] = round(leds_scaled[i].g);
			raw_led_data[3*i+2] = round(leds_scaled[i].b);
		}
	}
}

IRAM_ATTR void transmit_leds() {
	// Wait here if previous frame transmission has not yet completed
	rmt_tx_wait_all_done( tx_chan_a, portMAX_DELAY );
	rmt_tx_wait_all_done( tx_chan_b, portMAX_DELAY );

	// Clear the 8-bit buffer	
	memset( raw_led_data, 0, NUM_LEDS*3 );

	// Quantize the floating point color to 8-bit with temporal dithering
	//
	// This allows the 8-bit LEDs to emulate the look of a higher bit-depth using Persistence of Vision
	// The contents of the floating point CRGBF "leds" array are downsampled into pseudo-random 8-bit
	// dither patterns hundreds of times per second. Your eyes see these patterns as a smooth, higher
	// dynamic range image with deeper color reproduction.
	quantize_color_error( configuration.temporal_dithering.value.u32 );

	// Get to safety, THE PHOTONS ARE COMING!!!
	if( filesystem_ready == true ){
		rmt_transmit( tx_chan_a, led_encoder_a, raw_led_data, (sizeof(raw_led_data) >> 1), &tx_config );
		rmt_transmit( tx_chan_b, led_encoder_b, raw_led_data+((NUM_LEDS>>1)*3), (sizeof(raw_led_data) >> 1), &tx_config );
	}
}