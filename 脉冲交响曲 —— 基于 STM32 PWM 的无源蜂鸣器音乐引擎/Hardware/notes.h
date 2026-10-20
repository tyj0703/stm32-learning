#ifndef __notes_H
#define __notes_H

//单个音符的转化

//C调

//低音区（对应简谱数字下方加点）
#define L1  262		//DO
#define L2  294		//RE
#define L3  330		//Mi
#define L4  349		//Fa
#define L5  392		//Sol
#define L6  440		//La
#define L7  494		//Si

//中音区（对应简谱数字）
#define M1  523
#define M2  587
#define M3  659
#define M4  698
#define M5  784
#define M6  880
#define M7  988

//高音区（对应简谱数字上方加点）
#define H1  1046
#define H2  1174
#define H3  1318
#define H4  1396
#define H5  1568
#define H6  1760
#define H7  1976

//休止符
#define REST  0

#ifndef T
#define T 600
#endif

#define Q   T          			//四分音符,对应单个数字
#define H   (2*T)      			//二分音符，对应数字后加-
#define W   (4*T)      			//全音符，对应数字加---
#define D   (T + T/2)  			//附点四分音符
#define E   (T/2)      			// 八分音符，对应数字加下划线
#define DE  (T/2 + T/4)			// 附点八分音符
#define S   (T/4)      			// 十六分音符，对应数字加两条下划线
#define DS  (T/4 + T/8)			// 附点十六分音符

#define T3	(T/3)				//三连音

#endif
