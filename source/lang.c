#include "lang.h"

#include <3ds/types.h>
#include <3ds/result.h>
#include <3ds/services/cfgu.h>

Language userlang = LANGUAGE_EN;

void updateSystemLanguage(void)
{
	Result ret;
	u8 language = 0;

	cfguInit();
	ret = CFGU_GetConfigInfoBlk2(1, 0xA0002, &language);
	cfguExit();
	
	if (R_SUCCEEDED(ret))
	{
		switch (language)
		{
			case CFG_LANGUAGE_JP: userlang = LANGUAGE_JP; break;
			case CFG_LANGUAGE_FR: userlang = LANGUAGE_FR; break;
			case CFG_LANGUAGE_IT: userlang = LANGUAGE_IT; break;
			case CFG_LANGUAGE_DE: userlang = LANGUAGE_DE; break;
			case CFG_LANGUAGE_ES: userlang = LANGUAGE_ES; break;
			case CFG_LANGUAGE_KO: userlang = LANGUAGE_KR; break;
			default:              userlang = LANGUAGE_EN; break;
		}
	}
}
