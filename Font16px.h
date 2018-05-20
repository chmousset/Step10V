#ifndef Font16pxH
#define Font16pxH

#ifndef SMfgTypes
#define SMfgTypes

/*======= binar input =======*/
#define b2b(b7u,b6u,b5u,b4u,b3u,b2u,b1u,b0u) ((unsigned char)((b7u)*128u + (b6u)*64u + (b5u)*32u + (b4u)*16u + (b3u)*8u + (b2u)*4u + (b1u)*2u + (b0u)))

/*============================================================================*/
/* You have to manualy set correct types TCDATA and TCLISTP                   */
/* for your platform and compiler.                                            */
/*                                                                            */
/* Keil C51 example:                                                          */
/* Character data (TCDATA) are stored in code memory,                         */
/* array of pointers to characters (TCLISTP) is stored in code memory         */
/* and pointers are pointing into code memory.                                */
/*                                                                            */
/* typedef unsigned char code TCDATA;                                         */
/* typedef TCDATA * code TCLISTP;                                             */
/*============================================================================*/

typedef unsigned char TCDATA;
typedef TCDATA* TCLISTP;

#endif

/*======= Character pointers table =======*/
extern TCLISTP Font16pxBold[256];

/*======= Characters data =======*/
extern TCDATA Font16pxBold_32[5];
extern TCDATA Font16pxBold_33[5];
extern TCDATA Font16pxBold_34[11];
extern TCDATA Font16pxBold_35[17];
extern TCDATA Font16pxBold_36[13];
extern TCDATA Font16pxBold_37[17];
extern TCDATA Font16pxBold_38[15];
extern TCDATA Font16pxBold_39[5];
extern TCDATA Font16pxBold_40[7];
extern TCDATA Font16pxBold_41[7];
extern TCDATA Font16pxBold_42[11];
extern TCDATA Font16pxBold_43[13];
extern TCDATA Font16pxBold_44[5];
extern TCDATA Font16pxBold_45[11];
extern TCDATA Font16pxBold_46[5];
extern TCDATA Font16pxBold_47[9];
extern TCDATA Font16pxBold_48[13];
extern TCDATA Font16pxBold_49[13];
extern TCDATA Font16pxBold_50[13];
extern TCDATA Font16pxBold_51[13];
extern TCDATA Font16pxBold_52[13];
extern TCDATA Font16pxBold_53[13];
extern TCDATA Font16pxBold_54[13];
extern TCDATA Font16pxBold_55[13];
extern TCDATA Font16pxBold_56[13];
extern TCDATA Font16pxBold_57[13];
extern TCDATA Font16pxBold_58[5];
extern TCDATA Font16pxBold_59[7];
extern TCDATA Font16pxBold_60[13];
extern TCDATA Font16pxBold_61[11];
extern TCDATA Font16pxBold_62[13];
extern TCDATA Font16pxBold_63[13];
extern TCDATA Font16pxBold_64[17];
extern TCDATA Font16pxBold_65[17];
extern TCDATA Font16pxBold_66[13];
extern TCDATA Font16pxBold_67[13];
extern TCDATA Font16pxBold_68[13];
extern TCDATA Font16pxBold_69[13];
extern TCDATA Font16pxBold_70[13];
extern TCDATA Font16pxBold_71[15];
extern TCDATA Font16pxBold_72[13];
extern TCDATA Font16pxBold_73[5];
extern TCDATA Font16pxBold_74[11];
extern TCDATA Font16pxBold_75[15];
extern TCDATA Font16pxBold_76[13];
extern TCDATA Font16pxBold_77[15];
extern TCDATA Font16pxBold_78[15];
extern TCDATA Font16pxBold_79[15];
extern TCDATA Font16pxBold_80[13];
extern TCDATA Font16pxBold_81[15];
extern TCDATA Font16pxBold_82[13];
extern TCDATA Font16pxBold_83[13];
extern TCDATA Font16pxBold_84[13];
extern TCDATA Font16pxBold_85[15];
extern TCDATA Font16pxBold_86[15];
extern TCDATA Font16pxBold_87[15];
extern TCDATA Font16pxBold_88[13];
extern TCDATA Font16pxBold_89[17];
extern TCDATA Font16pxBold_90[15];
extern TCDATA Font16pxBold_91[7];
extern TCDATA Font16pxBold_92[9];
extern TCDATA Font16pxBold_93[7];
extern TCDATA Font16pxBold_94[11];
extern TCDATA Font16pxBold_95[13];
extern TCDATA Font16pxBold_96[9];
extern TCDATA Font16pxBold_97[13];
extern TCDATA Font16pxBold_98[11];
extern TCDATA Font16pxBold_99[11];
extern TCDATA Font16pxBold_100[11];
extern TCDATA Font16pxBold_101[11];
extern TCDATA Font16pxBold_102[9];
extern TCDATA Font16pxBold_103[11];
extern TCDATA Font16pxBold_104[11];
extern TCDATA Font16pxBold_105[5];
extern TCDATA Font16pxBold_106[7];
extern TCDATA Font16pxBold_107[11];
extern TCDATA Font16pxBold_108[5];
extern TCDATA Font16pxBold_109[17];
extern TCDATA Font16pxBold_110[11];
extern TCDATA Font16pxBold_111[11];
extern TCDATA Font16pxBold_112[11];
extern TCDATA Font16pxBold_113[11];
extern TCDATA Font16pxBold_114[9];
extern TCDATA Font16pxBold_115[11];
extern TCDATA Font16pxBold_116[9];
extern TCDATA Font16pxBold_117[11];
extern TCDATA Font16pxBold_118[17];
extern TCDATA Font16pxBold_119[15];
extern TCDATA Font16pxBold_120[13];
extern TCDATA Font16pxBold_121[15];
extern TCDATA Font16pxBold_122[11];
extern TCDATA Font16pxBold_123[9];
extern TCDATA Font16pxBold_124[5];
extern TCDATA Font16pxBold_125[9];
extern TCDATA Font16pxBold_126[11];
extern TCDATA Font16pxBold_138[13];
extern TCDATA Font16pxBold_141[13];
extern TCDATA Font16pxBold_142[15];
extern TCDATA Font16pxBold_154[11];
extern TCDATA Font16pxBold_157[11];
extern TCDATA Font16pxBold_158[11];
extern TCDATA Font16pxBold_167[13];
extern TCDATA Font16pxBold_169[17];
extern TCDATA Font16pxBold_193[17];
extern TCDATA Font16pxBold_200[13];
extern TCDATA Font16pxBold_201[13];
extern TCDATA Font16pxBold_204[13];
extern TCDATA Font16pxBold_205[7];
extern TCDATA Font16pxBold_207[15];
extern TCDATA Font16pxBold_210[15];
extern TCDATA Font16pxBold_211[15];
extern TCDATA Font16pxBold_216[13];
extern TCDATA Font16pxBold_217[15];
extern TCDATA Font16pxBold_218[15];
extern TCDATA Font16pxBold_221[17];
extern TCDATA Font16pxBold_225[13];
extern TCDATA Font16pxBold_232[11];
extern TCDATA Font16pxBold_233[11];
extern TCDATA Font16pxBold_236[11];
extern TCDATA Font16pxBold_237[7];
extern TCDATA Font16pxBold_239[15];
extern TCDATA Font16pxBold_242[11];
extern TCDATA Font16pxBold_243[11];
extern TCDATA Font16pxBold_248[9];
extern TCDATA Font16pxBold_249[11];
extern TCDATA Font16pxBold_250[11];
extern TCDATA Font16pxBold_253[15];
extern TCDATA Font16pxBold_blank[11];

#endif
