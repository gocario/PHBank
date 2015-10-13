#ifndef POKEMON_HPP
#define POKEMON_HPP

#include "pkbank.hpp"

namespace Stat
{
	typedef enum stat_e
	{
		HP,
		ATK,
		DEF,
		SPE,
		SPA,
		SPD
	} stat_e;
}
typedef Stat::stat_e stat_e;


class Pokemon
{
	public:
		static void computeChecksum(pkm_t* pkm);

		/* Quicker */
		static u16 PSV(pkm_t* pkm);
		static u8 TSV(pkm_t* pkm);
		static bool isShiny(pkm_t* pkm);
		static bool isInfected(pkm_t* pkm);
		static bool isCured(pkm_t* pkm);
		static bool isGen6(pkm_t* pkm);

		static u8 level(pkm_t* pkm);
		static u16 stat(u16 species, u8 IV, u8 EV, u8 nature, u8 level, u8 stat, u8 form);
		static u16 HP(pkm_t* pkm);
		static u16 ATK(pkm_t* pkm);
		static u16 DEF(pkm_t* pkm);
		static u16 SPA(pkm_t* pkm);
		static u16 SPD(pkm_t* pkm);
		static u16 SPE(pkm_t* pkm);
		static u8 HPType(pkm_t* pkm);
		

	public:
		// Region A
		static u32 encryptionKey(pkm_t* pkm);
		static u16 sanity(pkm_t* pkm);
		static u16 checksum(pkm_t* pkm);
		static u16 speciesID(pkm_t* pkm);
		static u16 itemID(pkm_t* pkm);
		static u16 TID(pkm_t* pkm);
		static u16 SID(pkm_t* pkm);
		static u32 EXP(pkm_t* pkm);
		static u8 ability(pkm_t* pkm);
		static u8 abilityNumber(pkm_t* pkm);
		static u8 trainingBagHits(pkm_t* pkm);
		static u8 triningBag(pkm_t* pkm);
		static u32 PID(pkm_t* pkm);
		static u8 nature(pkm_t* pkm);
		static bool fatefulEncounter(pkm_t* pkm);
		static u8 gender(pkm_t* pkm);
		static u8 form(pkm_t* pkm);
		static u8 EV_HP(pkm_t* pkm);
		static u8 EV_ATK(pkm_t* pkm);
		static u8 EV_DEF(pkm_t* pkm);
		static u8 EV_SPE(pkm_t* pkm);
		static u8 EV_SPA(pkm_t* pkm);
		static u8 EV_SPD(pkm_t* pkm);
		static u8 CNT_Cool(pkm_t* pkm);
		static u8 CNT_Beauty(pkm_t* pkm);
		static u8 CNT_Cute(pkm_t* pkm);
		static u8 CNT_Smart(pkm_t* pkm);
		static u8 CNT_Tough(pkm_t* pkm);
		static u8 CNT_Sheen(pkm_t* pkm);
		static bool circle(pkm_t* pkm);
		static bool triangle(pkm_t* pkm);
		static bool square(pkm_t* pkm);
		static bool heart(pkm_t* pkm);
		static bool star(pkm_t* pkm);
		static bool diamond(pkm_t* pkm);
		static u8 PKRS_days(pkm_t* pkm);
		static u8 PKRS_strain(pkm_t* pkm);
		static bool unused0(pkm_t* pkm);
		static bool unused1(pkm_t* pkm);
		static bool ST1_SPA(pkm_t* pkm);
		static bool ST1_HP(pkm_t* pkm);
		static bool ST1_ATK(pkm_t* pkm);
		static bool ST1_SPD(pkm_t* pkm);
		static bool ST1_SPE(pkm_t* pkm);
		static bool ST1_DEF(pkm_t* pkm);
		static bool ST2_SPA(pkm_t* pkm);
		static bool ST2_HP(pkm_t* pkm);
		static bool ST2_ATK(pkm_t* pkm);
		static bool ST2_SPD(pkm_t* pkm);
		static bool ST2_SPE(pkm_t* pkm);
		static bool ST2_DEF(pkm_t* pkm);
		static bool ST3_SPA(pkm_t* pkm);
		static bool ST3_HP(pkm_t* pkm);
		static bool ST3_ATK(pkm_t* pkm);
		static bool ST3_SPD(pkm_t* pkm);
		static bool ST3_SPE(pkm_t* pkm);
		static bool ST3_DEF(pkm_t* pkm);
		static bool ST4_1(pkm_t* pkm);
		static bool ST5_1(pkm_t* pkm);
		static bool ST5_2(pkm_t* pkm);
		static bool ST5_3(pkm_t* pkm);
		static bool ST5_4(pkm_t* pkm);
		static bool ST6_1(pkm_t* pkm);
		static bool ST6_2(pkm_t* pkm);
		static bool ST6_3(pkm_t* pkm);
		static bool ST7_1(pkm_t* pkm);
		static bool ST7_2(pkm_t* pkm);
		static bool ST7_3(pkm_t* pkm);
		static bool ST8_1(pkm_t* pkm);
		static bool RIB0_0(pkm_t* pkm);
		static bool RIB0_1(pkm_t* pkm);
		static bool RIB0_2(pkm_t* pkm);
		static bool RIB0_3(pkm_t* pkm);
		static bool RIB0_4(pkm_t* pkm);
		static bool RIB0_5(pkm_t* pkm);
		static bool RIB0_6(pkm_t* pkm);
		static bool RIB0_7(pkm_t* pkm);
		static bool RIB1_0(pkm_t* pkm);
		static bool RIB1_1(pkm_t* pkm);
		static bool RIB1_2(pkm_t* pkm);
		static bool RIB1_3(pkm_t* pkm);
		static bool RIB1_4(pkm_t* pkm);
		static bool RIB1_5(pkm_t* pkm);
		static bool RIB1_6(pkm_t* pkm);
		static bool RIB1_7(pkm_t* pkm);
		static bool RIB2_0(pkm_t* pkm);
		static bool RIB2_1(pkm_t* pkm);
		static bool RIB2_2(pkm_t* pkm);
		static bool RIB2_3(pkm_t* pkm);
		static bool RIB2_4(pkm_t* pkm);
		static bool RIB2_5(pkm_t* pkm);
		static bool RIB2_6(pkm_t* pkm);
		static bool RIB2_7(pkm_t* pkm);
		static bool RIB3_0(pkm_t* pkm);
		static bool RIB3_1(pkm_t* pkm);
		static bool RIB3_2(pkm_t* pkm);
		static bool RIB3_3(pkm_t* pkm);
		static bool RIB3_4(pkm_t* pkm);
		static bool RIB3_5(pkm_t* pkm);
		static bool RIB3_6(pkm_t* pkm);
		static bool RIB3_7(pkm_t* pkm);
		static bool RIB4_0(pkm_t* pkm);
		static bool RIB4_1(pkm_t* pkm);
		static bool RIB4_2(pkm_t* pkm);
		static bool RIB4_3(pkm_t* pkm);
		static bool RIB4_4(pkm_t* pkm);
		static bool RIB4_5(pkm_t* pkm);
		static bool RIB4_6(pkm_t* pkm);
		static bool RIB4_7(pkm_t* pkm);
		static bool RIB5_0(pkm_t* pkm);
		static bool RIB5_1(pkm_t* pkm);
		static bool RIB5_2(pkm_t* pkm);
		static bool RIB5_3(pkm_t* pkm);
		static bool RIB5_4(pkm_t* pkm);
		static bool RIB5_5(pkm_t* pkm);
		static bool RIB5_6(pkm_t* pkm);
		static bool RIB5_7(pkm_t* pkm);
		static u8 _0x36(pkm_t* pkm);
		static u8 _0x37(pkm_t* pkm);
		static u8 contestCount(pkm_t* pkm);
		static u8 battleCount(pkm_t* pkm);
		static bool dist1(pkm_t* pkm);
		static bool dist2(pkm_t* pkm);
		static bool dist3(pkm_t* pkm);
		static bool dist4(pkm_t* pkm);
		static bool dist5(pkm_t* pkm);
		static bool dist6(pkm_t* pkm);
		static bool dist7(pkm_t* pkm);
		static bool dist8(pkm_t* pkm);
		static u8 _0x3B(pkm_t* pkm);
		static u8 _0x3C(pkm_t* pkm);
		static u8 _0x3D(pkm_t* pkm);
		static u8 _0x3E(pkm_t* pkm);
		static u8 _0x3F(pkm_t* pkm);
		
		// Region B
		static u8* nickname(pkm_t* pkm);
		static u16 move1(pkm_t* pkm);
		static u16 move2(pkm_t* pkm);
		static u16 move3(pkm_t* pkm);
		static u16 move4(pkm_t* pkm);
		static u8 move1PP(pkm_t* pkm);
		static u8 move2PP(pkm_t* pkm);
		static u8 move3PP(pkm_t* pkm);
		static u8 move4PP(pkm_t* pkm);
		static u8 move1PPPlus(pkm_t* pkm);
		static u8 move2PPPlus(pkm_t* pkm);
		static u8 move3PPPlus(pkm_t* pkm);
		static u8 move4PPPlus(pkm_t* pkm);
		static u16 relearnMove1(pkm_t* pkm);
		static u16 relearnMove2(pkm_t* pkm);
		static u16 relearnMove3(pkm_t* pkm);
		static u16 relearnMove4(pkm_t* pkm);
		static bool secretSuperTraining(pkm_t* pkm);
		static u8 _0x73(pkm_t* pkm);
		static u8 IV_HP(pkm_t* pkm);
		static u8 IV_ATK(pkm_t* pkm);
		static u8 IV_DEF(pkm_t* pkm);
		static u8 IV_SPE(pkm_t* pkm);
		static u8 IV_SPA(pkm_t* pkm);
		static u8 IV_SPD(pkm_t* pkm);
		static bool isEgg(pkm_t* pkm);
		static bool isNicknamed(pkm_t* pkm);

		// Region C
		static u8* HT_name(pkm_t* pkm);
		static u8 HT_gender(pkm_t* pkm);
		static u8 currentHandler(pkm_t* pkm);
		static u8 geo1Region(pkm_t* pkm);
		static u8 geo1Country(pkm_t* pkm);
		static u8 geo2Region(pkm_t* pkm);
		static u8 geo2Country(pkm_t* pkm);
		static u8 geo3Region(pkm_t* pkm);
		static u8 geo3Country(pkm_t* pkm);
		static u8 geo4Region(pkm_t* pkm);
		static u8 geo4Country(pkm_t* pkm);
		static u8 geo5Region(pkm_t* pkm);
		static u8 geo5Country(pkm_t* pkm);
		static u8 _0x9E(pkm_t* pkm);
		static u8 _0x9F(pkm_t* pkm);
		static u8 _0xA0(pkm_t* pkm);
		static u8 _0xA1(pkm_t* pkm);
		static u8 HT_friendship(pkm_t* pkm);
		static u8 HT_affection(pkm_t* pkm);
		static u8 HT_intensity(pkm_t* pkm);
		static u8 HT_memory(pkm_t* pkm);
		static u8 HT_feeling(pkm_t* pkm);
		static u8 _0xA7(pkm_t* pkm);
		static u16 HT_textVar(pkm_t* pkm);
		static u8 _0xAA(pkm_t* pkm);
		static u8 _0xAB(pkm_t* pkm);
		static u8 _0xAC(pkm_t* pkm);
		static u8 _0xAD(pkm_t* pkm);
		static u8 fullness(pkm_t* pkm);
		static u8 enjoyment(pkm_t* pkm);

		// Region D
		static u8* OT_name(pkm_t* pkm);
		static u8 OT_friendship(pkm_t* pkm);
		static u8 OT_affection(pkm_t* pkm);
		static u8 OT_intensity(pkm_t* pkm);
		static u8 OT_memory(pkm_t* pkm);
		static u16 OT_textVar(pkm_t* pkm);
		static u8 OT_feeling(pkm_t* pkm);
		static u8 eggYear(pkm_t* pkm);
		static u8 eggMonth(pkm_t* pkm);
		static u8 eggDay(pkm_t* pkm);
		static u8 metYear(pkm_t* pkm);
		static u8 metMonth(pkm_t* pkm);
		static u8 metDay(pkm_t* pkm);
		static u8 _0xD7(pkm_t* pkm);
		static u16 eggLocation(pkm_t* pkm);
		static u16 metLocation(pkm_t* pkm);
		static u8 ball(pkm_t* pkm);
		static u8 metLevel(pkm_t* pkm);
		static u8 OT_gender(pkm_t* pkm);
		static u8 encounterType(pkm_t* pkm);
		static u8 version(pkm_t* pkm);
		static u8 country(pkm_t* pkm);
		static u8 region(pkm_t* pkm);
		static u8 consoleRegion(pkm_t* pkm);
		static u8 language(pkm_t* pkm);

		// Battle stats
		// static u8 statLevel(pkm_t* pkm);
		// static u8 statCurrentHP(pkm_t* pkm);
		// static u8 statMaxHP(pkm_t* pkm);
		// static u8 statATK(pkm_t* pkm);
		// static u8 statDEF(pkm_t* pkm);
		// static u8 statSPE(pkm_t* pkm);
		// static u8 statSPA(pkm_t* pkm);
		// static u8 statSPD(pkm_t* pkm);

		/* Abstract values */
		static u8 markings(pkm_t* pkm);
		static u8 PKRS(pkm_t* pkm);
		static u8 ST1(pkm_t* pkm);
		static u8 ST2(pkm_t* pkm);
		static u8 ST3(pkm_t* pkm);
		static u8 ST4(pkm_t* pkm);
		static u8 RIB0(pkm_t* pkm);
		static u8 RIB1(pkm_t* pkm);
		static u8 RIB2(pkm_t* pkm);
		static u8 RIB3(pkm_t* pkm);
		static u8 RIB4(pkm_t* pkm);
		static u8 RIB5(pkm_t* pkm);
		static u8 dist(pkm_t* pkm);
		static u32 IV32(pkm_t* pkm);


	public:
		// Region A
		static void encryptionKey(pkm_t* pkm, u32 v);
		// static void sanity(pkm_t* pkm, u16 v);
		static void checksum(pkm_t* pkm, u16 v);
		static void speciesID(pkm_t* pkm, u16 v);
		static void itemID(pkm_t* pkm, u16 v);
		static void TID(pkm_t* pkm, u16 v);
		static void SID(pkm_t* pkm, u16 v);
		static void EXP(pkm_t* pkm, u32 v);
		static void ability(pkm_t* pkm, u8 v);
		static void abilityNumber(pkm_t* pkm, u8 v);
		static void trainingBagHits(pkm_t* pkm, u8 v);
		static void triningBag(pkm_t* pkm, u8 v);
		static void PID(pkm_t* pkm, u32 v);
		static void nature(pkm_t* pkm, u8 v);
		static void fatefulEncounter(pkm_t* pkm, bool v);
		static void gender(pkm_t* pkm, u8 v);
		static void form(pkm_t* pkm, u8 v);
		static void EV_HP(pkm_t* pkm, u8 v);
		static void EV_ATK(pkm_t* pkm, u8 v);
		static void EV_DEF(pkm_t* pkm, u8 v);
		static void EV_SPE(pkm_t* pkm, u8 v);
		static void EV_SPA(pkm_t* pkm, u8 v);
		static void EV_SPD(pkm_t* pkm, u8 v);
		static void CNT_Cool(pkm_t* pkm, u8 v);
		static void CNT_Beauty(pkm_t* pkm, u8 v);
		static void CNT_Cute(pkm_t* pkm, u8 v);
		static void CNT_Smart(pkm_t* pkm, u8 v);
		static void CNT_Tough(pkm_t* pkm, u8 v);
		static void CNT_Sheen(pkm_t* pkm, u8 v);
		static void circle(pkm_t* pkm, bool v);
		static void triangle(pkm_t* pkm, bool v);
		static void square(pkm_t* pkm, bool v);
		static void heart(pkm_t* pkm, bool v);
		static void star(pkm_t* pkm, bool v);
		static void diamond(pkm_t* pkm, bool v);
		static void PKRS_days(pkm_t* pkm, u8 v);
		static void PKRS_strain(pkm_t* pkm, u8 v);
		static void unused0(pkm_t* pkm, bool v);
		static void unused1(pkm_t* pkm, bool v);
		static void ST1_SPA(pkm_t* pkm, bool v);
		static void ST1_HP(pkm_t* pkm, bool v);
		static void ST1_ATK(pkm_t* pkm, bool v);
		static void ST1_SPD(pkm_t* pkm, bool v);
		static void ST1_SPE(pkm_t* pkm, bool v);
		static void ST1_DEF(pkm_t* pkm, bool v);
		static void ST2_SPA(pkm_t* pkm, bool v);
		static void ST2_HP(pkm_t* pkm, bool v);
		static void ST2_ATK(pkm_t* pkm, bool v);
		static void ST2_SPD(pkm_t* pkm, bool v);
		static void ST2_SPE(pkm_t* pkm, bool v);
		static void ST2_DEF(pkm_t* pkm, bool v);
		static void ST3_SPA(pkm_t* pkm, bool v);
		static void ST3_HP(pkm_t* pkm, bool v);
		static void ST3_ATK(pkm_t* pkm, bool v);
		static void ST3_SPD(pkm_t* pkm, bool v);
		static void ST3_SPE(pkm_t* pkm, bool v);
		static void ST3_DEF(pkm_t* pkm, bool v);
		static void ST4_1(pkm_t* pkm, bool v);
		static void ST5_1(pkm_t* pkm, bool v);
		static void ST5_2(pkm_t* pkm, bool v);
		static void ST5_3(pkm_t* pkm, bool v);
		static void ST5_4(pkm_t* pkm, bool v);
		static void ST6_1(pkm_t* pkm, bool v);
		static void ST6_2(pkm_t* pkm, bool v);
		static void ST6_3(pkm_t* pkm, bool v);
		static void ST7_1(pkm_t* pkm, bool v);
		static void ST7_2(pkm_t* pkm, bool v);
		static void ST7_3(pkm_t* pkm, bool v);
		static void ST8_1(pkm_t* pkm, bool v);
		static void RIB0_0(pkm_t* pkm, bool v);
		static void RIB0_1(pkm_t* pkm, bool v);
		static void RIB0_2(pkm_t* pkm, bool v);
		static void RIB0_3(pkm_t* pkm, bool v);
		static void RIB0_4(pkm_t* pkm, bool v);
		static void RIB0_5(pkm_t* pkm, bool v);
		static void RIB0_6(pkm_t* pkm, bool v);
		static void RIB0_7(pkm_t* pkm, bool v);
		static void RIB1_0(pkm_t* pkm, bool v);
		static void RIB1_1(pkm_t* pkm, bool v);
		static void RIB1_2(pkm_t* pkm, bool v);
		static void RIB1_3(pkm_t* pkm, bool v);
		static void RIB1_4(pkm_t* pkm, bool v);
		static void RIB1_5(pkm_t* pkm, bool v);
		static void RIB1_6(pkm_t* pkm, bool v);
		static void RIB1_7(pkm_t* pkm, bool v);
		static void RIB2_0(pkm_t* pkm, bool v);
		static void RIB2_1(pkm_t* pkm, bool v);
		static void RIB2_2(pkm_t* pkm, bool v);
		static void RIB2_3(pkm_t* pkm, bool v);
		static void RIB2_4(pkm_t* pkm, bool v);
		static void RIB2_5(pkm_t* pkm, bool v);
		static void RIB2_6(pkm_t* pkm, bool v);
		static void RIB2_7(pkm_t* pkm, bool v);
		static void RIB3_0(pkm_t* pkm, bool v);
		static void RIB3_1(pkm_t* pkm, bool v);
		static void RIB3_2(pkm_t* pkm, bool v);
		static void RIB3_3(pkm_t* pkm, bool v);
		static void RIB3_4(pkm_t* pkm, bool v);
		static void RIB3_5(pkm_t* pkm, bool v);
		static void RIB3_6(pkm_t* pkm, bool v);
		static void RIB3_7(pkm_t* pkm, bool v);
		static void RIB4_0(pkm_t* pkm, bool v);
		static void RIB4_1(pkm_t* pkm, bool v);
		static void RIB4_2(pkm_t* pkm, bool v);
		static void RIB4_3(pkm_t* pkm, bool v);
		static void RIB4_4(pkm_t* pkm, bool v);
		static void RIB4_5(pkm_t* pkm, bool v);
		static void RIB4_6(pkm_t* pkm, bool v);
		static void RIB4_7(pkm_t* pkm, bool v);
		static void RIB5_0(pkm_t* pkm, bool v);
		static void RIB5_1(pkm_t* pkm, bool v);
		static void RIB5_2(pkm_t* pkm, bool v);
		static void RIB5_3(pkm_t* pkm, bool v);
		static void RIB5_4(pkm_t* pkm, bool v);
		static void RIB5_5(pkm_t* pkm, bool v);
		static void RIB5_6(pkm_t* pkm, bool v);
		static void RIB5_7(pkm_t* pkm, bool v);
		static void _0x36(pkm_t* pkm, u8 v);
		static void _0x37(pkm_t* pkm, u8 v);
		static void contestCount(pkm_t* pkm, u8 v);
		static void battleCount(pkm_t* pkm, u8 v);
		static void dist1(pkm_t* pkm, bool v);
		static void dist2(pkm_t* pkm, bool v);
		static void dist3(pkm_t* pkm, bool v);
		static void dist4(pkm_t* pkm, bool v);
		static void dist5(pkm_t* pkm, bool v);
		static void dist6(pkm_t* pkm, bool v);
		static void dist7(pkm_t* pkm, bool v);
		static void dist8(pkm_t* pkm, bool v);
		static void _0x3B(pkm_t* pkm, u8 v);
		static void _0x3C(pkm_t* pkm, u8 v);
		static void _0x3D(pkm_t* pkm, u8 v);
		static void _0x3E(pkm_t* pkm, u8 v);
		static void _0x3F(pkm_t* pkm, u8 v);

		// Region B
		static void nickname(pkm_t* pkm, u8* v);
		static void move1(pkm_t* pkm, u16 v);
		static void move2(pkm_t* pkm, u16 v);
		static void move3(pkm_t* pkm, u16 v);
		static void move4(pkm_t* pkm, u16 v);
		static void move1PP(pkm_t* pkm, u8 v);
		static void move2PP(pkm_t* pkm, u8 v);
		static void move3PP(pkm_t* pkm, u8 v);
		static void move4PP(pkm_t* pkm, u8 v);
		static void move1PPPlus(pkm_t* pkm, u8 v);
		static void move2PPPlus(pkm_t* pkm, u8 v);
		static void move3PPPlus(pkm_t* pkm, u8 v);
		static void move4PPPlus(pkm_t* pkm, u8 v);
		static void relearnMove1(pkm_t* pkm, u16 v);
		static void relearnMove2(pkm_t* pkm, u16 v);
		static void relearnMove3(pkm_t* pkm, u16 v);
		static void relearnMove4(pkm_t* pkm, u16 v);
		static void secretSuperTraining(pkm_t* pkm, bool v);
		static void _0x73(pkm_t* pkm, u8 v);
		static void IV_HP(pkm_t* pkm, u8 v);
		static void IV_ATK(pkm_t* pkm, u8 v);
		static void IV_DEF(pkm_t* pkm, u8 v);
		static void IV_SPE(pkm_t* pkm, u8 v);
		static void IV_SPA(pkm_t* pkm, u8 v);
		static void IV_SPD(pkm_t* pkm, u8 v);
		static void isEgg(pkm_t* pkm, bool v);
		static void isNicknamed(pkm_t* pkm, bool v);

		// Region C
		static void HT_name(pkm_t* pkm, u8* v);
		static void HT_gender(pkm_t* pkm, u8 v);
		static void currentHandler(pkm_t* pkm, u8 v);
		static void geo1Region(pkm_t* pkm, u8 v);
		static void geo1Country(pkm_t* pkm, u8 v);
		static void geo2Region(pkm_t* pkm, u8 v);
		static void geo2Country(pkm_t* pkm, u8 v);
		static void geo3Region(pkm_t* pkm, u8 v);
		static void geo3Country(pkm_t* pkm, u8 v);
		static void geo4Region(pkm_t* pkm, u8 v);
		static void geo4Country(pkm_t* pkm, u8 v);
		static void geo5Region(pkm_t* pkm, u8 v);
		static void geo5Country(pkm_t* pkm, u8 v);
		static void _0x9E(pkm_t* pkm, u8 v);
		static void _0x9F(pkm_t* pkm, u8 v);
		static void _0xA0(pkm_t* pkm, u8 v);
		static void _0xA1(pkm_t* pkm, u8 v);
		static void HT_friendship(pkm_t* pkm, u8 v);
		static void HT_affection(pkm_t* pkm, u8 v);
		static void HT_intensity(pkm_t* pkm, u8 v);
		static void HT_memory(pkm_t* pkm, u8 v);
		static void HT_feeling(pkm_t* pkm, u8 v);
		static void _0xA7(pkm_t* pkm, u8 v);
		static void HT_textVar(pkm_t* pkm, u16 v);
		static void _0xAA(pkm_t* pkm, u8 v);
		static void _0xAB(pkm_t* pkm, u8 v);
		static void _0xAC(pkm_t* pkm, u8 v);
		static void _0xAD(pkm_t* pkm, u8 v);
		static void fullness(pkm_t* pkm, u8 v);
		static void enjoyment(pkm_t* pkm, u8 v);

		// Region D
		static void OT_name(pkm_t* pkm, u8* v);
		static void OT_friendship(pkm_t* pkm, u8 v);
		static void OT_affection(pkm_t* pkm, u8 v);
		static void OT_intensity(pkm_t* pkm, u8 v);
		static void OT_memory(pkm_t* pkm, u8 v);
		static void OT_textVar(pkm_t* pkm, u16 v);
		static void OT_feeling(pkm_t* pkm, u8 v);
		static void eggYear(pkm_t* pkm, u8 v);
		static void eggMonth(pkm_t* pkm, u8 v);
		static void eggDay(pkm_t* pkm, u8 v);
		static void metYear(pkm_t* pkm, u8 v);
		static void metMonth(pkm_t* pkm, u8 v);
		static void metDay(pkm_t* pkm, u8 v);
		static void _0xD7(pkm_t* pkm, u8 v);
		static void eggLocation(pkm_t* pkm, u16 v);
		static void metLocation(pkm_t* pkm, u16 v);
		static void ball(pkm_t* pkm, u8 v);
		static void metLevel(pkm_t* pkm, u8 v);
		static void OT_gender(pkm_t* pkm, u8 v);
		static void encounterType(pkm_t* pkm, u8 v);
		static void version(pkm_t* pkm, u8 v);
		static void country(pkm_t* pkm, u8 v);
		static void region(pkm_t* pkm, u8 v);
		static void consoleRegion(pkm_t* pkm, u8 v);
		static void language(pkm_t* pkm, u8 v);

		// Battle stats
		// static void statLevel(pkm_t* pkm, u8 v);
		// static void statCurrentHP(pkm_t* pkm, u8 v);
		// static void statMaxHP(pkm_t* pkm, u8 v);
		// static void statATK(pkm_t* pkm, u8 v);
		// static void statDEF(pkm_t* pkm, u8 v);
		// static void statSPE(pkm_t* pkm, u8 v);
		// static void statSPA(pkm_t* pkm, u8 v);
		// static void statSPD(pkm_t* pkm, u8 v);

		/* Abstract values */
		static void markings(pkm_t* pkm, u8 v);
		static void PKRS(pkm_t* pkm, u8 v);
		static void ST1(pkm_t* pkm, u8 v);
		static void ST2(pkm_t* pkm, u8 v);
		static void ST3(pkm_t* pkm, u8 v);
		static void ST4(pkm_t* pkm, u8 v);
		static void RIB0(pkm_t* pkm, u8 v);
		static void RIB1(pkm_t* pkm, u8 v);
		static void RIB2(pkm_t* pkm, u8 v);
		static void RIB3(pkm_t* pkm, u8 v);
		static void RIB4(pkm_t* pkm, u8 v);
		static void RIB5(pkm_t* pkm, u8 v);
		static void dist(pkm_t* pkm, u8 v);
		static void IV32(pkm_t* pkm, u32 v);
};


#endif // POKEMON_HPP