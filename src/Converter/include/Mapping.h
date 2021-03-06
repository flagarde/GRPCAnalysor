#pragma once
#include <array>

//      (I Axis)
// 7	|46|63|61|59|58|56|54|52|  	
// 6	|43|45|62|60|57|55|53|51|  	     
// 5	|42|44|47|48|49|50|41|40|  	     
// 4	|32|33|34|35|36|37|38|39|  	     
// 3	|31|30|29|28|27|26|25|24|  	     TOP VIEW (ASICs SIDE)
// 2	|20|19|16|15|14|21|22|23|  	
// 1	|18|00|02|04|06|09|11|13| 
// 0	|17|01|03|05|07|08|10|12|   
//     0  1  2  3  4  5  6  7    (J Axis)  ----->
//				|	 |
//				|DIFF|
//				|____|	
//=====================================


const std::array<unsigned short,49> AsicShiftI={0,	0,	0,	0,	0,	8,	8,
	                                        8,	8,	16,	16,	16,	16,	24,
	                                        24,	24,	24,	32,	32,	32,	32,
	                                        40,	40,	40,	40,	48,	48,	48,
	                                        48,	56,	56,	56,	56,	64,	64,
  	                                        64,	64,	72,	72,	72,	72,	80,
	                                        80,	80,	80,	88,	88,	88,	88};
	                                              
const std::array<unsigned short,64>MapJLargeHR2={1,1,2,2,3,3,4,4,5,5,6,6,7,7,4,3,
                                                 2,0,0,1,0,5,6,7,7,6,5,4,3,2,1,0,
                                                 0,1,2,3,4,5,6,7,7,6,0,0,1,1,0,2,
                                                 3,4,5,7,7,6,6,5,5,4,4,3,3,2,2,1};
                                                 
const std::array<unsigned short,49>AsicShiftJ={0,       0,	8,	16,	24,	24,	16,
	                                       8,	0,	0,	8,	16,	24,	24,
	                                       16,	8,	0,	0,	8,	16,	24,
	                                       24,	16,	8,	0,	0,	8,	16,
	                                       24,	24,	16,	8,	0,	0,	8,
	                                       16,	24,	24,	16,	8,	0,	0,
	                                       8,	16,	24,	24,	16,	8,	0};
	                                            
const std::array<unsigned short,64> MapILargeHR2={1,0,1,0,1,0,1,0,0,1,0,1,0,1,2,2,
                                                  2,0,1,2,2,2,2,2,3,3,3,3,3,3,3,3,
                                                  4,4,4,4,4,4,4,4,5,5,5,6,5,6,7,5,
                                                  5,5,5,6,7,6,7,6,7,6,7,7,6,7,6,7};

const std::array<std::array<unsigned short,8>,8> MapChannelLargeHR2{{ 
                                                                        {17, 1, 3, 5, 7, 8,10,12},
                                                                        {18, 0, 2, 4, 6, 9,11,13},
                                                                        {20,19,16,15,14,21,22,23},
                                                                        {31,30,29,28,27,26,25,24},
                                                                        {32,33,34,35,36,37,38,39},
                                                                        {42,44,47,48,49,50,41,40},
                                                                        {43,45,62,60,57,55,53,51},
                                                                        {46,63,61,59,58,56,54,52}
                                                                    }};


const std::array<std::array<unsigned short,4>,12> MapAsicLargeHR2{{
                                                                        { 4, 3, 2, 1},
                                                                        { 5, 6, 7, 8},
                                                                         {12,11,10, 9},
                                                                         {13,14,15,16},
                                                                         {20,19,18,17},
                                                                         {21,22,23,24},
                                                                         {28,27,26,25},
                                                                         {29,30,31,32},
                                                                         {36,35,34,33},
                                                                         {37,38,39,40},
                                                                         {44,43,42,41},
                                                                         {45,46,47,48}
                                                                   }};                                           

