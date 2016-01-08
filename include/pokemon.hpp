#pragma once
/**
 * @file pokemon.hpp
 */
#ifndef POKEMON_HPP
#define POKEMON_HPP

#include "save.hpp"

namespace Stat
{
	typedef enum stat_e
	{
		HP = 0,
		ATK = 1,
		DEF = 2,
		SPE = 3,
		SPA = 4,
		SPD = 5
	} stat_e;
}
typedef Stat::stat_e stat_e;

namespace Pokemon
{
	/* Checksum */
	void computeChecksum(pkm_s* pkm);

	/* Quicker */
	u16 PSV(pkm_s* pkm);
	u16 TSV(u16 _TID, u16 _SID);
	u16 TSV(pkm_s* pkm);
	bool isShiny(pkm_s* pkm, u16 _TID, u16 _SID);
	bool isShiny(pkm_s* pkm);
	bool isInfected(pkm_s* pkm);
	bool isCured(pkm_s* pkm);
	bool isKalosBorn(pkm_s* pkm);

	u8 level(pkm_s* pkm);
	u16 stat(u16 species, u8 IV, u8 EV, u8 nature, u8 level, u8 stat, u8 form);
	u16 HP(pkm_s* pkm);
	u16 ATK(pkm_s* pkm);
	u16 DEF(pkm_s* pkm);
	u16 SPA(pkm_s* pkm);
	u16 SPD(pkm_s* pkm);
	u16 SPE(pkm_s* pkm);
	u16 HP(pkm_s* pkm, u8 level);
	u16 ATK(pkm_s* pkm, u8 level);
	u16 DEF(pkm_s* pkm, u8 level);
	u16 SPA(pkm_s* pkm, u8 level);
	u16 SPD(pkm_s* pkm, u8 level);
	u16 SPE(pkm_s* pkm, u8 level);
	u8 HPType(pkm_s* pkm);
		

		// Region A
	u32 encryptionKey(pkm_s* pkm);
	u16 sanity(pkm_s* pkm);
	u16 checksum(pkm_s* pkm);
	u16 speciesID(pkm_s* pkm);
	u16 itemID(pkm_s* pkm);
	u16 TID(pkm_s* pkm);
	u16 SID(pkm_s* pkm);
	u32 EXP(pkm_s* pkm);
	u8 ability(pkm_s* pkm);
	u8 abilityNumber(pkm_s* pkm);
	u8 trainingBagHits(pkm_s* pkm);
	u8 triningBag(pkm_s* pkm);
	u32 PID(pkm_s* pkm);
	u8 nature(pkm_s* pkm);
	bool fatefulEncounter(pkm_s* pkm);
	u8 gender(pkm_s* pkm);
	u8 form(pkm_s* pkm);
	u8 EV_HP(pkm_s* pkm);
	u8 EV_ATK(pkm_s* pkm);
	u8 EV_DEF(pkm_s* pkm);
	u8 EV_SPE(pkm_s* pkm);
	u8 EV_SPA(pkm_s* pkm);
	u8 EV_SPD(pkm_s* pkm);
	u8 CNT_Cool(pkm_s* pkm);
	u8 CNT_Beauty(pkm_s* pkm);
	u8 CNT_Cute(pkm_s* pkm);
	u8 CNT_Smart(pkm_s* pkm);
	u8 CNT_Tough(pkm_s* pkm);
	u8 CNT_Sheen(pkm_s* pkm);
	bool circle(pkm_s* pkm);
	bool triangle(pkm_s* pkm);
	bool square(pkm_s* pkm);
	bool heart(pkm_s* pkm);
	bool star(pkm_s* pkm);
	bool diamond(pkm_s* pkm);
	u8 PKRS_days(pkm_s* pkm);
	u8 PKRS_strain(pkm_s* pkm);
	bool unused0(pkm_s* pkm);
	bool unused1(pkm_s* pkm);
	bool ST1_SPA(pkm_s* pkm);
	bool ST1_HP(pkm_s* pkm);
	bool ST1_ATK(pkm_s* pkm);
	bool ST1_SPD(pkm_s* pkm);
	bool ST1_SPE(pkm_s* pkm);
	bool ST1_DEF(pkm_s* pkm);
	bool ST2_SPA(pkm_s* pkm);
	bool ST2_HP(pkm_s* pkm);
	bool ST2_ATK(pkm_s* pkm);
	bool ST2_SPD(pkm_s* pkm);
	bool ST2_SPE(pkm_s* pkm);
	bool ST2_DEF(pkm_s* pkm);
	bool ST3_SPA(pkm_s* pkm);
	bool ST3_HP(pkm_s* pkm);
	bool ST3_ATK(pkm_s* pkm);
	bool ST3_SPD(pkm_s* pkm);
	bool ST3_SPE(pkm_s* pkm);
	bool ST3_DEF(pkm_s* pkm);
	bool ST4_1(pkm_s* pkm);
	bool ST5_1(pkm_s* pkm);
	bool ST5_2(pkm_s* pkm);
	bool ST5_3(pkm_s* pkm);
	bool ST5_4(pkm_s* pkm);
	bool ST6_1(pkm_s* pkm);
	bool ST6_2(pkm_s* pkm);
	bool ST6_3(pkm_s* pkm);
	bool ST7_1(pkm_s* pkm);
	bool ST7_2(pkm_s* pkm);
	bool ST7_3(pkm_s* pkm);
	bool ST8_1(pkm_s* pkm);
	bool RIB0_0(pkm_s* pkm);
	bool RIB0_1(pkm_s* pkm);
	bool RIB0_2(pkm_s* pkm);
	bool RIB0_3(pkm_s* pkm);
	bool RIB0_4(pkm_s* pkm);
	bool RIB0_5(pkm_s* pkm);
	bool RIB0_6(pkm_s* pkm);
	bool RIB0_7(pkm_s* pkm);
	bool RIB1_0(pkm_s* pkm);
	bool RIB1_1(pkm_s* pkm);
	bool RIB1_2(pkm_s* pkm);
	bool RIB1_3(pkm_s* pkm);
	bool RIB1_4(pkm_s* pkm);
	bool RIB1_5(pkm_s* pkm);
	bool RIB1_6(pkm_s* pkm);
	bool RIB1_7(pkm_s* pkm);
	bool RIB2_0(pkm_s* pkm);
	bool RIB2_1(pkm_s* pkm);
	bool RIB2_2(pkm_s* pkm);
	bool RIB2_3(pkm_s* pkm);
	bool RIB2_4(pkm_s* pkm);
	bool RIB2_5(pkm_s* pkm);
	bool RIB2_6(pkm_s* pkm);
	bool RIB2_7(pkm_s* pkm);
	bool RIB3_0(pkm_s* pkm);
	bool RIB3_1(pkm_s* pkm);
	bool RIB3_2(pkm_s* pkm);
	bool RIB3_3(pkm_s* pkm);
	bool RIB3_4(pkm_s* pkm);
	bool RIB3_5(pkm_s* pkm);
	bool RIB3_6(pkm_s* pkm);
	bool RIB3_7(pkm_s* pkm);
	bool RIB4_0(pkm_s* pkm);
	bool RIB4_1(pkm_s* pkm);
	bool RIB4_2(pkm_s* pkm);
	bool RIB4_3(pkm_s* pkm);
	bool RIB4_4(pkm_s* pkm);
	bool RIB4_5(pkm_s* pkm);
	bool RIB4_6(pkm_s* pkm);
	bool RIB4_7(pkm_s* pkm);
	bool RIB5_0(pkm_s* pkm);
	bool RIB5_1(pkm_s* pkm);
	bool RIB5_2(pkm_s* pkm);
	bool RIB5_3(pkm_s* pkm);
	bool RIB5_4(pkm_s* pkm);
	bool RIB5_5(pkm_s* pkm);
	bool RIB5_6(pkm_s* pkm);
	bool RIB5_7(pkm_s* pkm);
	u8 _0x36(pkm_s* pkm);
	u8 _0x37(pkm_s* pkm);
	u8 contestCount(pkm_s* pkm);
	u8 battleCount(pkm_s* pkm);
	bool dist1(pkm_s* pkm);
	bool dist2(pkm_s* pkm);
	bool dist3(pkm_s* pkm);
	bool dist4(pkm_s* pkm);
	bool dist5(pkm_s* pkm);
	bool dist6(pkm_s* pkm);
	bool dist7(pkm_s* pkm);
	bool dist8(pkm_s* pkm);
	u8 _0x3B(pkm_s* pkm);
	u8 _0x3C(pkm_s* pkm);
	u8 _0x3D(pkm_s* pkm);
	u8 _0x3E(pkm_s* pkm);
	u8 _0x3F(pkm_s* pkm);
		
	// Region B
	u16* NK_name(pkm_s* pkm);
	u16 move1(pkm_s* pkm);
	u16 move2(pkm_s* pkm);
	u16 move3(pkm_s* pkm);
	u16 move4(pkm_s* pkm);
	u8 move1PP(pkm_s* pkm);
	u8 move2PP(pkm_s* pkm);
	u8 move3PP(pkm_s* pkm);
	u8 move4PP(pkm_s* pkm);
	u8 move1PPPlus(pkm_s* pkm);
	u8 move2PPPlus(pkm_s* pkm);
	u8 move3PPPlus(pkm_s* pkm);
	u8 move4PPPlus(pkm_s* pkm);
	u16 relearnMove1(pkm_s* pkm);
	u16 relearnMove2(pkm_s* pkm);
	u16 relearnMove3(pkm_s* pkm);
	u16 relearnMove4(pkm_s* pkm);
	bool secretSuperTraining(pkm_s* pkm);
	u8 _0x73(pkm_s* pkm);
	u8 IV_HP(pkm_s* pkm);
	u8 IV_ATK(pkm_s* pkm);
	u8 IV_DEF(pkm_s* pkm);
	u8 IV_SPE(pkm_s* pkm);
	u8 IV_SPA(pkm_s* pkm);
	u8 IV_SPD(pkm_s* pkm);
	bool isEgg(pkm_s* pkm);
	bool isNicknamed(pkm_s* pkm);

	// Region C
	u16* HT_name(pkm_s* pkm);
	u8 HT_gender(pkm_s* pkm);
	u8 currentHandler(pkm_s* pkm);
	u8 geo1Region(pkm_s* pkm);
	u8 geo1Country(pkm_s* pkm);
	u8 geo2Region(pkm_s* pkm);
	u8 geo2Country(pkm_s* pkm);
	u8 geo3Region(pkm_s* pkm);
	u8 geo3Country(pkm_s* pkm);
	u8 geo4Region(pkm_s* pkm);
	u8 geo4Country(pkm_s* pkm);
	u8 geo5Region(pkm_s* pkm);
	u8 geo5Country(pkm_s* pkm);
	u8 _0x9E(pkm_s* pkm);
	u8 _0x9F(pkm_s* pkm);
	u8 _0xA0(pkm_s* pkm);
	u8 _0xA1(pkm_s* pkm);
	u8 HT_friendship(pkm_s* pkm);
	u8 HT_affection(pkm_s* pkm);
	u8 HT_intensity(pkm_s* pkm);
	u8 HT_memory(pkm_s* pkm);
	u8 HT_feeling(pkm_s* pkm);
	u8 _0xA7(pkm_s* pkm);
	u16 HT_textVar(pkm_s* pkm);
	u8 _0xAA(pkm_s* pkm);
	u8 _0xAB(pkm_s* pkm);
	u8 _0xAC(pkm_s* pkm);
	u8 _0xAD(pkm_s* pkm);
	u8 fullness(pkm_s* pkm);
	u8 enjoyment(pkm_s* pkm);

	// Region D
	u16* OT_name(pkm_s* pkm);
	u8 OT_friendship(pkm_s* pkm);
	u8 OT_affection(pkm_s* pkm);
	u8 OT_intensity(pkm_s* pkm);
	u8 OT_memory(pkm_s* pkm);
	u16 OT_textVar(pkm_s* pkm);
	u8 OT_feeling(pkm_s* pkm);
	u8 eggYear(pkm_s* pkm);
	u8 eggMonth(pkm_s* pkm);
	u8 eggDay(pkm_s* pkm);
	u8 metYear(pkm_s* pkm);
	u8 metMonth(pkm_s* pkm);
	u8 metDay(pkm_s* pkm);
	u8 _0xD7(pkm_s* pkm);
	u16 eggLocation(pkm_s* pkm);
	u16 metLocation(pkm_s* pkm);
	u8 ball(pkm_s* pkm);
	u8 metLevel(pkm_s* pkm);
	u8 OT_gender(pkm_s* pkm);
	u8 encounterType(pkm_s* pkm);
	u8 version(pkm_s* pkm);
	u8 country(pkm_s* pkm);
	u8 region(pkm_s* pkm);
	u8 consoleRegion(pkm_s* pkm);
	u8 language(pkm_s* pkm);

	// Battle stats
	u8 statLevel(pkm_s* pkm);
	u8 statCurrentHP(pkm_s* pkm);
	u8 statMaxHP(pkm_s* pkm);
	u8 statATK(pkm_s* pkm);
	u8 statDEF(pkm_s* pkm);
	u8 statSPE(pkm_s* pkm);
	u8 statSPA(pkm_s* pkm);
	u8 statSPD(pkm_s* pkm);

	/* Abstract values */
	u8 markings(pkm_s* pkm);
	u8 PKRS(pkm_s* pkm);
	u8 ST1(pkm_s* pkm);
	u8 ST2(pkm_s* pkm);
	u8 ST3(pkm_s* pkm);
	u8 ST4(pkm_s* pkm);
	u8 RIB0(pkm_s* pkm);
	u8 RIB1(pkm_s* pkm);
	u8 RIB2(pkm_s* pkm);
	u8 RIB3(pkm_s* pkm);
	u8 RIB4(pkm_s* pkm);
	u8 RIB5(pkm_s* pkm);
	u8 dist(pkm_s* pkm);
	u32 IV32(pkm_s* pkm);



	// Region A
	void encryptionKey(pkm_s* pkm, u32 v);
	void sanity(pkm_s* pkm, u16 v);
	void checksum(pkm_s* pkm, u16 v);
	void speciesID(pkm_s* pkm, u16 v);
	void itemID(pkm_s* pkm, u16 v);
	void TID(pkm_s* pkm, u16 v);
	void SID(pkm_s* pkm, u16 v);
	void EXP(pkm_s* pkm, u32 v);
	void ability(pkm_s* pkm, u8 v);
	void abilityNumber(pkm_s* pkm, u8 v);
	void trainingBagHits(pkm_s* pkm, u8 v);
	void triningBag(pkm_s* pkm, u8 v);
	void PID(pkm_s* pkm, u32 v);
	void nature(pkm_s* pkm, u8 v);
	void fatefulEncounter(pkm_s* pkm, bool v);
	void gender(pkm_s* pkm, u8 v);
	void form(pkm_s* pkm, u8 v);
	void EV_HP(pkm_s* pkm, u8 v);
	void EV_ATK(pkm_s* pkm, u8 v);
	void EV_DEF(pkm_s* pkm, u8 v);
	void EV_SPE(pkm_s* pkm, u8 v);
	void EV_SPA(pkm_s* pkm, u8 v);
	void EV_SPD(pkm_s* pkm, u8 v);
	void CNT_Cool(pkm_s* pkm, u8 v);
	void CNT_Beauty(pkm_s* pkm, u8 v);
	void CNT_Cute(pkm_s* pkm, u8 v);
	void CNT_Smart(pkm_s* pkm, u8 v);
	void CNT_Tough(pkm_s* pkm, u8 v);
	void CNT_Sheen(pkm_s* pkm, u8 v);
	void circle(pkm_s* pkm, bool v);
	void triangle(pkm_s* pkm, bool v);
	void square(pkm_s* pkm, bool v);
	void heart(pkm_s* pkm, bool v);
	void star(pkm_s* pkm, bool v);
	void diamond(pkm_s* pkm, bool v);
	void PKRS_days(pkm_s* pkm, u8 v);
	void PKRS_strain(pkm_s* pkm, u8 v);
	void unused0(pkm_s* pkm, bool v);
	void unused1(pkm_s* pkm, bool v);
	void ST1_SPA(pkm_s* pkm, bool v);
	void ST1_HP(pkm_s* pkm, bool v);
	void ST1_ATK(pkm_s* pkm, bool v);
	void ST1_SPD(pkm_s* pkm, bool v);
	void ST1_SPE(pkm_s* pkm, bool v);
	void ST1_DEF(pkm_s* pkm, bool v);
	void ST2_SPA(pkm_s* pkm, bool v);
	void ST2_HP(pkm_s* pkm, bool v);
	void ST2_ATK(pkm_s* pkm, bool v);
	void ST2_SPD(pkm_s* pkm, bool v);
	void ST2_SPE(pkm_s* pkm, bool v);
	void ST2_DEF(pkm_s* pkm, bool v);
	void ST3_SPA(pkm_s* pkm, bool v);
	void ST3_HP(pkm_s* pkm, bool v);
	void ST3_ATK(pkm_s* pkm, bool v);
	void ST3_SPD(pkm_s* pkm, bool v);
	void ST3_SPE(pkm_s* pkm, bool v);
	void ST3_DEF(pkm_s* pkm, bool v);
	void ST4_1(pkm_s* pkm, bool v);
	void ST5_1(pkm_s* pkm, bool v);
	void ST5_2(pkm_s* pkm, bool v);
	void ST5_3(pkm_s* pkm, bool v);
	void ST5_4(pkm_s* pkm, bool v);
	void ST6_1(pkm_s* pkm, bool v);
	void ST6_2(pkm_s* pkm, bool v);
	void ST6_3(pkm_s* pkm, bool v);
	void ST7_1(pkm_s* pkm, bool v);
	void ST7_2(pkm_s* pkm, bool v);
	void ST7_3(pkm_s* pkm, bool v);
	void ST8_1(pkm_s* pkm, bool v);
	void RIB0_0(pkm_s* pkm, bool v);
	void RIB0_1(pkm_s* pkm, bool v);
	void RIB0_2(pkm_s* pkm, bool v);
	void RIB0_3(pkm_s* pkm, bool v);
	void RIB0_4(pkm_s* pkm, bool v);
	void RIB0_5(pkm_s* pkm, bool v);
	void RIB0_6(pkm_s* pkm, bool v);
	void RIB0_7(pkm_s* pkm, bool v);
	void RIB1_0(pkm_s* pkm, bool v);
	void RIB1_1(pkm_s* pkm, bool v);
	void RIB1_2(pkm_s* pkm, bool v);
	void RIB1_3(pkm_s* pkm, bool v);
	void RIB1_4(pkm_s* pkm, bool v);
	void RIB1_5(pkm_s* pkm, bool v);
	void RIB1_6(pkm_s* pkm, bool v);
	void RIB1_7(pkm_s* pkm, bool v);
	void RIB2_0(pkm_s* pkm, bool v);
	void RIB2_1(pkm_s* pkm, bool v);
	void RIB2_2(pkm_s* pkm, bool v);
	void RIB2_3(pkm_s* pkm, bool v);
	void RIB2_4(pkm_s* pkm, bool v);
	void RIB2_5(pkm_s* pkm, bool v);
	void RIB2_6(pkm_s* pkm, bool v);
	void RIB2_7(pkm_s* pkm, bool v);
	void RIB3_0(pkm_s* pkm, bool v);
	void RIB3_1(pkm_s* pkm, bool v);
	void RIB3_2(pkm_s* pkm, bool v);
	void RIB3_3(pkm_s* pkm, bool v);
	void RIB3_4(pkm_s* pkm, bool v);
	void RIB3_5(pkm_s* pkm, bool v);
	void RIB3_6(pkm_s* pkm, bool v);
	void RIB3_7(pkm_s* pkm, bool v);
	void RIB4_0(pkm_s* pkm, bool v);
	void RIB4_1(pkm_s* pkm, bool v);
	void RIB4_2(pkm_s* pkm, bool v);
	void RIB4_3(pkm_s* pkm, bool v);
	void RIB4_4(pkm_s* pkm, bool v);
	void RIB4_5(pkm_s* pkm, bool v);
	void RIB4_6(pkm_s* pkm, bool v);
	void RIB4_7(pkm_s* pkm, bool v);
	void RIB5_0(pkm_s* pkm, bool v);
	void RIB5_1(pkm_s* pkm, bool v);
	void RIB5_2(pkm_s* pkm, bool v);
	void RIB5_3(pkm_s* pkm, bool v);
	void RIB5_4(pkm_s* pkm, bool v);
	void RIB5_5(pkm_s* pkm, bool v);
	void RIB5_6(pkm_s* pkm, bool v);
	void RIB5_7(pkm_s* pkm, bool v);
	void _0x36(pkm_s* pkm, u8 v);
	void _0x37(pkm_s* pkm, u8 v);
	void contestCount(pkm_s* pkm, u8 v);
	void battleCount(pkm_s* pkm, u8 v);
	void dist1(pkm_s* pkm, bool v);
	void dist2(pkm_s* pkm, bool v);
	void dist3(pkm_s* pkm, bool v);
	void dist4(pkm_s* pkm, bool v);
	void dist5(pkm_s* pkm, bool v);
	void dist6(pkm_s* pkm, bool v);
	void dist7(pkm_s* pkm, bool v);
	void dist8(pkm_s* pkm, bool v);
	void _0x3B(pkm_s* pkm, u8 v);
	void _0x3C(pkm_s* pkm, u8 v);
	void _0x3D(pkm_s* pkm, u8 v);
	void _0x3E(pkm_s* pkm, u8 v);
	void _0x3F(pkm_s* pkm, u8 v);

	// Region B
	void NK_name(pkm_s* pkm, u16* v);
	void move1(pkm_s* pkm, u16 v);
	void move2(pkm_s* pkm, u16 v);
	void move3(pkm_s* pkm, u16 v);
	void move4(pkm_s* pkm, u16 v);
	void move1PP(pkm_s* pkm, u8 v);
	void move2PP(pkm_s* pkm, u8 v);
	void move3PP(pkm_s* pkm, u8 v);
	void move4PP(pkm_s* pkm, u8 v);
	void move1PPPlus(pkm_s* pkm, u8 v);
	void move2PPPlus(pkm_s* pkm, u8 v);
	void move3PPPlus(pkm_s* pkm, u8 v);
	void move4PPPlus(pkm_s* pkm, u8 v);
	void relearnMove1(pkm_s* pkm, u16 v);
	void relearnMove2(pkm_s* pkm, u16 v);
	void relearnMove3(pkm_s* pkm, u16 v);
	void relearnMove4(pkm_s* pkm, u16 v);
	void secretSuperTraining(pkm_s* pkm, bool v);
	void _0x73(pkm_s* pkm, u8 v);
	void IV_HP(pkm_s* pkm, u8 v);
	void IV_ATK(pkm_s* pkm, u8 v);
	void IV_DEF(pkm_s* pkm, u8 v);
	void IV_SPE(pkm_s* pkm, u8 v);
	void IV_SPA(pkm_s* pkm, u8 v);
	void IV_SPD(pkm_s* pkm, u8 v);
	void isEgg(pkm_s* pkm, bool v);
	void isNicknamed(pkm_s* pkm, bool v);

	// Region C
	void HT_name(pkm_s* pkm, u16* v);
	void HT_gender(pkm_s* pkm, u8 v);
	void currentHandler(pkm_s* pkm, u8 v);
	void geo1Region(pkm_s* pkm, u8 v);
	void geo1Country(pkm_s* pkm, u8 v);
	void geo2Region(pkm_s* pkm, u8 v);
	void geo2Country(pkm_s* pkm, u8 v);
	void geo3Region(pkm_s* pkm, u8 v);
	void geo3Country(pkm_s* pkm, u8 v);
	void geo4Region(pkm_s* pkm, u8 v);
	void geo4Country(pkm_s* pkm, u8 v);
	void geo5Region(pkm_s* pkm, u8 v);
	void geo5Country(pkm_s* pkm, u8 v);
	void _0x9E(pkm_s* pkm, u8 v);
	void _0x9F(pkm_s* pkm, u8 v);
	void _0xA0(pkm_s* pkm, u8 v);
	void _0xA1(pkm_s* pkm, u8 v);
	void HT_friendship(pkm_s* pkm, u8 v);
	void HT_affection(pkm_s* pkm, u8 v);
	void HT_intensity(pkm_s* pkm, u8 v);
	void HT_memory(pkm_s* pkm, u8 v);
	void HT_feeling(pkm_s* pkm, u8 v);
	void _0xA7(pkm_s* pkm, u8 v);
	void HT_textVar(pkm_s* pkm, u16 v);
	void _0xAA(pkm_s* pkm, u8 v);
	void _0xAB(pkm_s* pkm, u8 v);
	void _0xAC(pkm_s* pkm, u8 v);
	void _0xAD(pkm_s* pkm, u8 v);
	void fullness(pkm_s* pkm, u8 v);
	void enjoyment(pkm_s* pkm, u8 v);

	// Region D
	void OT_name(pkm_s* pkm, u16* v);
	void OT_friendship(pkm_s* pkm, u8 v);
	void OT_affection(pkm_s* pkm, u8 v);
	void OT_intensity(pkm_s* pkm, u8 v);
	void OT_memory(pkm_s* pkm, u8 v);
	void OT_textVar(pkm_s* pkm, u16 v);
	void OT_feeling(pkm_s* pkm, u8 v);
	void eggYear(pkm_s* pkm, u8 v);
	void eggMonth(pkm_s* pkm, u8 v);
	void eggDay(pkm_s* pkm, u8 v);
	void metYear(pkm_s* pkm, u8 v);
	void metMonth(pkm_s* pkm, u8 v);
	void metDay(pkm_s* pkm, u8 v);
	void _0xD7(pkm_s* pkm, u8 v);
	void eggLocation(pkm_s* pkm, u16 v);
	void metLocation(pkm_s* pkm, u16 v);
	void ball(pkm_s* pkm, u8 v);
	void metLevel(pkm_s* pkm, u8 v);
	void OT_gender(pkm_s* pkm, u8 v);
	void encounterType(pkm_s* pkm, u8 v);
	void version(pkm_s* pkm, u8 v);
	void country(pkm_s* pkm, u8 v);
	void region(pkm_s* pkm, u8 v);
	void consoleRegion(pkm_s* pkm, u8 v);
	void language(pkm_s* pkm, u8 v);

	// Battle stats
	void statLevel(pkm_s* pkm, u8 v);
	void statCurrentHP(pkm_s* pkm, u8 v);
	void statMaxHP(pkm_s* pkm, u8 v);
	void statATK(pkm_s* pkm, u8 v);
	void statDEF(pkm_s* pkm, u8 v);
	void statSPE(pkm_s* pkm, u8 v);
	void statSPA(pkm_s* pkm, u8 v);
	void statSPD(pkm_s* pkm, u8 v);

	/* Abstract values */
	void markings(pkm_s* pkm, u8 v);
	void PKRS(pkm_s* pkm, u8 v);
	void ST1(pkm_s* pkm, u8 v);
	void ST2(pkm_s* pkm, u8 v);
	void ST3(pkm_s* pkm, u8 v);
	void ST4(pkm_s* pkm, u8 v);
	void RIB0(pkm_s* pkm, u8 v);
	void RIB1(pkm_s* pkm, u8 v);
	void RIB2(pkm_s* pkm, u8 v);
	void RIB3(pkm_s* pkm, u8 v);
	void RIB4(pkm_s* pkm, u8 v);
	void RIB5(pkm_s* pkm, u8 v);
	void dist(pkm_s* pkm, u8 v);
	void IV32(pkm_s* pkm, u32 v);
};

#endif // POKEMON_HPP
