#include "pokemon.hpp"

// Checksum
void Pokemon::computeChecksum(pkm_t* pkm)
{
	if (pkm->modified)
	{
		u16 chk = 0;
		for (int i = 8; i < 0xe8; i += 2)
			chk += *(u16*)(pkm->pk6 + i);

		Pokemon::checksum(pkm, chk);
	}
}

// Quickers
u16 Pokemon::PSV(pkm_t* pkm) { return ((PID(pkm) >> 16) ^(PID(pkm) & 0xffff)) >> 4; }
u8 Pokemon::TSV(pkm_t* pkm) { return TID(pkm) ^ SID(pkm) >> 4; }
bool Pokemon::isShiny(pkm_t* pkm) { return TSV(pkm) == PSV(pkm); }
bool Pokemon::isInfected(pkm_t* pkm) { return PKRS_strain(pkm) > 0; }
bool Pokemon::isCured(pkm_t* pkm) { return PKRS_days(pkm) == 0 && PKRS_strain(pkm) > 0; }
bool Pokemon::isGen6(pkm_t* pkm) { return version(pkm) >= 24; }

u8 Pokemon::level(pkm_t* pkm)
{
	// TODO: Implement!
	return 0;
}
u16 Pokemon::HP(pkm_t* pkm)
{
	// TODO: Implement!
	return 0;
}
u16 Pokemon::ATK(pkm_t* pkm)
{
	// TODO: Implement!
	return 0;
}
u16 Pokemon::DEF(pkm_t* pkm)
{
	// TODO: Implement!
	return 0;
}
u16 Pokemon::SPA(pkm_t* pkm)
{
	// TODO: Implement!
	return 0;
}
u16 Pokemon::SPD(pkm_t* pkm)
{
	// TODO: Implement!
	return 0;
}
u16 Pokemon::SPE(pkm_t* pkm)
{
	// TODO: Implement!
	return 0;
}
u8 Pokemon::hiddenPower(pkm_t* pkm)
{
	return (15 * ((IV_HP(pkm) & 1) + 2 * (IV_ATK(pkm) & 1) + 4 * (IV_DEF(pkm)) + 8 * (IV_SPE(pkm) & 1) + 16 * (IV_SPA(pkm) & 1) + 32 * (IV_SPD(pkm) & 1))) / 63;
}

 /** GETTERS **/

// Region A
u32 Pokemon::encryptionKey(pkm_t* pkm) { return *(u32*)(pkm->pk6 + 0x00); }
u16 Pokemon::sanity(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x02); }
u16 Pokemon::checksum(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x06); }
u16 Pokemon::speciesID(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x08); }
u16 Pokemon::itemID(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x0a); }
u16 Pokemon::TID(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x0c); }
u16 Pokemon::SID(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x0e); }
u32 Pokemon::EXP(pkm_t* pkm) { return *(u32*)(pkm->pk6 + 0x10); }
u8 Pokemon::ability(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x14); }
u8 Pokemon::abilityNumber(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x15); }
u8 Pokemon::trainingBagHits(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x16); }
u8 Pokemon::triningBag(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x17); }
u32 Pokemon::PID(pkm_t* pkm) { return *(u32*)(pkm->pk6 + 0x18); }
u8 Pokemon::nature(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x1c); }
bool Pokemon::fatefulEncounter(pkm_t* pkm) { return ((*(u8*)(pkm->pk6 + 0x1d)) & 1) == 1; }
u8 Pokemon::gender(pkm_t* pkm) { return ((*(u8*)(pkm->pk6 + 0x1d)) >> 1) & 0x3; }
u8 Pokemon::form(pkm_t* pkm) { return (*(u8*)(pkm->pk6 + 0x1d)) >> 3; }
u8 Pokemon::EV_HP(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x1e); }
u8 Pokemon::EV_ATK(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x1f); }
u8 Pokemon::EV_DEF(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x20); }
u8 Pokemon::EV_SPE(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x21); }
u8 Pokemon::EV_SPA(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x22); }
u8 Pokemon::EV_SPD(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x23); }
u8 Pokemon::CNT_Cool(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x24); }
u8 Pokemon::CNT_Beauty(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x25); }
u8 Pokemon::CNT_Cute(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x26); }
u8 Pokemon::CNT_Smart(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x27); }
u8 Pokemon::CNT_Tough(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x28); }
u8 Pokemon::CNT_Sheen(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x29); }
bool Pokemon::circle(pkm_t* pkm) { return (Pokemon::markings(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::triangle(pkm_t* pkm) { return (Pokemon::markings(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::square(pkm_t* pkm) { return (Pokemon::markings(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::heart(pkm_t* pkm) { return (Pokemon::markings(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::star(pkm_t* pkm) { return (Pokemon::markings(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::diamond(pkm_t* pkm) { return (Pokemon::markings(pkm) & (1 << 5)) == (1 << 5); }
u8 Pokemon::PKRS_days(pkm_t* pkm) { return Pokemon::PKRS(pkm) & 0xF; }
u8 Pokemon::PKRS_strain(pkm_t* pkm) { return Pokemon::PKRS(pkm) >> 4; }
bool Pokemon::unused0(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::unused1(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::ST1_SPA(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::ST1_HP(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::ST1_ATK(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::ST1_SPD(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::ST1_SPE(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::ST1_DEF(pkm_t* pkm) { return (Pokemon::ST1(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::ST2_SPA(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::ST2_HP(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::ST2_ATK(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::ST2_SPD(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::ST2_SPE(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::ST2_DEF(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::ST3_SPA(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::ST3_HP(pkm_t* pkm) { return (Pokemon::ST2(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::ST3_ATK(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::ST3_SPD(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::ST3_SPE(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::ST3_DEF(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::ST4_1(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::ST5_1(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::ST5_2(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::ST5_3(pkm_t* pkm) { return (Pokemon::ST3(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::ST5_4(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::ST6_1(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::ST6_2(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::ST6_3(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::ST7_1(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::ST7_2(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::ST7_3(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::ST8_1(pkm_t* pkm) { return (Pokemon::ST4(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::RIB0_0(pkm_t* pkm) { return (RIB0(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::RIB0_1(pkm_t* pkm) { return (RIB0(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::RIB0_2(pkm_t* pkm) { return (RIB0(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::RIB0_3(pkm_t* pkm) { return (RIB0(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::RIB0_4(pkm_t* pkm) { return (RIB0(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::RIB0_5(pkm_t* pkm) { return (RIB0(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::RIB0_6(pkm_t* pkm) { return (RIB0(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::RIB0_7(pkm_t* pkm) { return (RIB0(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::RIB1_0(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::RIB1_1(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::RIB1_2(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::RIB1_3(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::RIB1_4(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::RIB1_5(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::RIB1_6(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::RIB1_7(pkm_t* pkm) { return (Pokemon::RIB1(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::RIB2_0(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::RIB2_1(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::RIB2_2(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::RIB2_3(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::RIB2_4(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::RIB2_5(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::RIB2_6(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::RIB2_7(pkm_t* pkm) { return (Pokemon::RIB2(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::RIB3_0(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::RIB3_1(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::RIB3_2(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::RIB3_3(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::RIB3_4(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::RIB3_5(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::RIB3_6(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::RIB3_7(pkm_t* pkm) { return (Pokemon::RIB3(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::RIB4_0(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::RIB4_1(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::RIB4_2(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::RIB4_3(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::RIB4_4(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::RIB4_5(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::RIB4_6(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::RIB4_7(pkm_t* pkm) { return (Pokemon::RIB4(pkm) & (1 << 7)) == (1 << 7); }
bool Pokemon::RIB5_0(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::RIB5_1(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::RIB5_2(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::RIB5_3(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::RIB5_4(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::RIB5_5(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::RIB5_6(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::RIB5_7(pkm_t* pkm) { return (Pokemon::RIB5(pkm) & (1 << 7)) == (1 << 7); }
u8 _0x36(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x36); }
u8 _0x37(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x37); }
u8 contestCount(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x38); }
u8 battleCount(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x39); }
bool Pokemon::dist1(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 0)) == (1 << 0); }
bool Pokemon::dist2(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 1)) == (1 << 1); }
bool Pokemon::dist3(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 2)) == (1 << 2); }
bool Pokemon::dist4(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 3)) == (1 << 3); }
bool Pokemon::dist5(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 4)) == (1 << 4); }
bool Pokemon::dist6(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 5)) == (1 << 5); }
bool Pokemon::dist7(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 6)) == (1 << 6); }
bool Pokemon::dist8(pkm_t* pkm) { return (Pokemon::dist(pkm) & (1 << 7)) == (1 << 7); }
u8 Pokemon::_0x3B(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x3b); }
u8 Pokemon::_0x3C(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x3c); }
u8 Pokemon::_0x3D(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x3d); }
u8 Pokemon::_0x3E(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x3e); }
u8 Pokemon::_0x3F(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x3f); }

// Region B
u8* Pokemon::nickname(pkm_t* pkm) { return (u8*)(pkm->pk6 + 0x18); }
u16 Pokemon::move1(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x5a); }
u16 Pokemon::move2(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x5c); }
u16 Pokemon::move3(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x5e); }
u16 Pokemon::move4(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x60); }
u8 Pokemon::move1PPPlus(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x62); }
u8 Pokemon::move2PPPlus(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x63); }
u8 Pokemon::move3PPPlus(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x64); }
u8 Pokemon::move4PPPlus(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x65); }
u8 Pokemon::move1PP(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x66); }
u8 Pokemon::move2PP(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x67); }
u8 Pokemon::move3PP(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x68); }
u8 Pokemon::move4PP(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x69); }
u16 Pokemon::relearnMove1(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x6a); }
u16 Pokemon::relearnMove2(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x6c); }
u16 Pokemon::relearnMove3(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x6e); }
u16 Pokemon::relearnMove4(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0x70); }
bool Pokemon::secretSuperTraining(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x72) == 1; }
u8 Pokemon::_0x73(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x73); }
u8 Pokemon::IV_HP(pkm_t* pkm) { return (Pokemon::IV32(pkm) >> 00) & 0x1f; }
u8 Pokemon::IV_ATK(pkm_t* pkm) { return (Pokemon::IV32(pkm) >> 05) & 0x1f; }
u8 Pokemon::IV_DEF(pkm_t* pkm) { return (Pokemon::IV32(pkm) >> 10) & 0x1f; }
u8 Pokemon::IV_SPE(pkm_t* pkm) { return (Pokemon::IV32(pkm) >> 15) & 0x1f; }
u8 Pokemon::IV_SPA(pkm_t* pkm) { return (Pokemon::IV32(pkm) >> 20) & 0x1f; }
u8 Pokemon::IV_SPD(pkm_t* pkm) { return (Pokemon::IV32(pkm) >> 25) & 0x1f; }
bool Pokemon::isEgg(pkm_t* pkm) { return ((Pokemon::IV32(pkm) >> 30) & 0x1) == 1; }
bool Pokemon::isNicknamed(pkm_t* pkm) { return ((Pokemon::IV32(pkm) >> 31) & 0x1) == 1; }

// Region C
u8* Pokemon::HT_name(pkm_t* pkm) { return (u8*)(pkm->pk6 + 0x78); }
u8 Pokemon::HT_gender(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x92); }
u8 Pokemon::currentHandler(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x93); }
u8 Pokemon::geo1Region(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x94); }
u8 Pokemon::geo1Country(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x95); }
u8 Pokemon::geo2Region(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x96); }
u8 Pokemon::geo2Country(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x97); }
u8 Pokemon::geo3Region(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x98); }
u8 Pokemon::geo3Country(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x99); }
u8 Pokemon::geo4Region(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x9a); }
u8 Pokemon::geo4Country(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x9b); }
u8 Pokemon::geo5Region(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x9c); }
u8 Pokemon::geo5Country(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x9d); }
u8 Pokemon::_0x9E(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x9e); }
u8 Pokemon::_0x9F(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x9f); }
u8 Pokemon::_0xA0(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa0); }
u8 Pokemon::_0xA1(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa1); }
u8 Pokemon::HT_friendship(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa2); }
u8 Pokemon::HT_affection(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa3); }
u8 Pokemon::HT_intensity(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa4); }
u8 Pokemon::HT_memory(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa5); }
u8 Pokemon::HT_feeling(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa6); }
u8 Pokemon::_0xA7(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xa7); }
u16 Pokemon::HT_textVar(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0xa8); }
u8 Pokemon::_0xAA(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xaa); }
u8 Pokemon::_0xAB(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xab); }
u8 Pokemon::_0xAC(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xac); }
u8 Pokemon::_0xAD(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xad); }
u8 Pokemon::fullness(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xae); }
u8 Pokemon::enjoyment(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xaf); }

// Region D
u8* Pokemon::OT_name(pkm_t* pkm) { return *(u8**)(pkm->pk6 + 0xb0); }
u8 Pokemon::OT_friendship(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xca); }
u8 Pokemon::OT_affection(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xcb); }
u8 Pokemon::OT_intensity(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xcc); }
u8 Pokemon::OT_memory(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xcd); }
u16 Pokemon::OT_textVar(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0xce); }
u8 Pokemon::OT_feeling(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd0); }
u8 Pokemon::eggYear(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd1); }
u8 Pokemon::eggMonth(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd2); }
u8 Pokemon::eggDay(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd3); }
u8 Pokemon::metYear(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd4); }
u8 Pokemon::metMonth(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd5); }
u8 Pokemon::metDay(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd6); }
u8 Pokemon::_0xD7(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xd7); }
u16 Pokemon::eggLocation(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0xd8); }
u16 Pokemon::metLocation(pkm_t* pkm) { return *(u16*)(pkm->pk6 + 0xda); }
u8 Pokemon::ball(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xdc); }
u8 Pokemon::metLevel(pkm_t* pkm) { return (*(u8*)(pkm->pk6 + 0xdd)) & ~0x80; }
u8 Pokemon::OT_gender(pkm_t* pkm) { return (*(u8*)(pkm->pk6 + 0xdd)) >> 7; }
u8 Pokemon::encounterType(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xde); }
u8 Pokemon::version(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xdf); }
u8 Pokemon::country(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xe0); }
u8 Pokemon::region(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xe1); }
u8 Pokemon::consoleRegion(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xe2); }
u8 Pokemon::language(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xe3); }

// Battle stats
// u8 Pokemon::statLevel(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xec); }
// u8 Pokemon::statCurrentHP(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xf0); }
// u8 Pokemon::statMaxHP(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xf2); }
// u8 Pokemon::statATK(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xf4); }
// u8 Pokemon::statDEF(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xf6); }
// u8 Pokemon::statSPE(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xf8); }
// u8 Pokemon::statSPA(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xfa); }
// u8 Pokemon::statSPD(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0xfc); }

// Abstract values
u8 Pokemon::markings(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x2a); }
u8 Pokemon::PKRS(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x2b); }
u8 Pokemon::ST1(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x2c); }
u8 Pokemon::ST2(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x2d); }
u8 Pokemon::ST3(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x2e); }
u8 Pokemon::ST4(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x2f); }
u8 Pokemon::RIB0(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x30); }
u8 Pokemon::RIB1(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x31); }
u8 Pokemon::RIB2(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x32); }
u8 Pokemon::RIB3(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x33); }
u8 Pokemon::RIB4(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x34); }
u8 Pokemon::RIB5(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x35); }
u8 Pokemon::dist(pkm_t* pkm) { return *(u8*)(pkm->pk6 + 0x3a); }
u32 Pokemon::IV32(pkm_t* pkm) { return *(u32*)(pkm->pk6 + 0x74); }



/** SETTERS **/

// Region A
void Pokemon::encryptionKey(pkm_t* pkm, u32 v) { *(u32*)(pkm->pk6 + 0x00) = v; }
// void Pokemon::sanity(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x02) = v; }
void Pokemon::checksum(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x06) = v; }
void Pokemon::speciesID(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x08) = v; }
void Pokemon::itemID(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x0a) = v; }
void Pokemon::TID(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x0c) = v; }
void Pokemon::SID(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x0e) = v; }
void Pokemon::EXP(pkm_t* pkm, u32 v) { *(u32*)(pkm->pk6 + 0x10) = v; }
void Pokemon::ability(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x14) = v; }
void Pokemon::abilityNumber(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x15) = v; }
void Pokemon::trainingBagHits(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x16) = v; }
void Pokemon::triningBag(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x17) = v; }
void Pokemon::PID(pkm_t* pkm, u32 v) { *(u32*)(pkm->pk6 + 0x18) = v; }
void Pokemon::nature(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x1c) = v; }
void Pokemon::fatefulEncounter(pkm_t* pkm, bool v) { *(u8*)(pkm->pk6 + 0x1d) = (Pokemon::fatefulEncounter(pkm) & ~0x01) | (v ? 1 : 0); }
void Pokemon::gender(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x1d) = (Pokemon::gender(pkm) & ~0x06) | ((v ? 1 : 0) << 1); }
void Pokemon::form(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x1d) = (Pokemon::form(pkm) & ~0x07) | ((v ? 1 : 0) << 3); }
void Pokemon::EV_HP(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x1e) = v; }
void Pokemon::EV_ATK(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x1f) = v; }
void Pokemon::EV_DEF(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x20) = v; }
void Pokemon::EV_SPE(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x21) = v; }
void Pokemon::EV_SPA(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x22) = v; }
void Pokemon::EV_SPD(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x23) = v; }
void Pokemon::CNT_Cool(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x24) = v; }
void Pokemon::CNT_Beauty(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x25) = v; }
void Pokemon::CNT_Cute(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x26) = v; }
void Pokemon::CNT_Smart(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x27) = v; }
void Pokemon::CNT_Tough(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x28) = v; }
void Pokemon::CNT_Sheen(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x29) = v; }
void Pokemon::circle(pkm_t* pkm, bool v) { Pokemon::markings(pkm, (Pokemon::markings(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::triangle(pkm_t* pkm, bool v) { Pokemon::markings(pkm, (Pokemon::markings(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::square(pkm_t* pkm, bool v) { Pokemon::markings(pkm, (Pokemon::markings(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::heart(pkm_t* pkm, bool v) { Pokemon::markings(pkm, (Pokemon::markings(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::star(pkm_t* pkm, bool v) { Pokemon::markings(pkm, (Pokemon::markings(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::diamond(pkm_t* pkm, bool v) { Pokemon::markings(pkm, (Pokemon::markings(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::PKRS_days(pkm_t* pkm, u8 v) { Pokemon::PKRS(pkm, (Pokemon::PKRS(pkm) & ~0xF) | v); }
void Pokemon::PKRS_strain(pkm_t* pkm, u8 v) { Pokemon::PKRS(pkm, (Pokemon::PKRS(pkm) & 0xF) | (v << 4)); }
void Pokemon::unused0(pkm_t* pkm, bool v) { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::unused1(pkm_t* pkm, bool v) { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::ST1_SPA(pkm_t* pkm, bool v) { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::ST1_HP(pkm_t* pkm, bool v)  { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::ST1_ATK(pkm_t* pkm, bool v) { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::ST1_SPD(pkm_t* pkm, bool v) { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::ST1_SPE(pkm_t* pkm, bool v) { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::ST1_DEF(pkm_t* pkm, bool v) { Pokemon::ST1(pkm, (Pokemon::ST1(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::ST2_SPA(pkm_t* pkm, bool v) { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::ST2_HP(pkm_t* pkm, bool v)  { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::ST2_ATK(pkm_t* pkm, bool v) { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::ST2_SPD(pkm_t* pkm, bool v) { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::ST2_SPE(pkm_t* pkm, bool v) { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::ST2_DEF(pkm_t* pkm, bool v) { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::ST3_SPA(pkm_t* pkm, bool v) { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::ST3_HP(pkm_t* pkm, bool v)  { Pokemon::ST2(pkm, (Pokemon::ST2(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::ST3_ATK(pkm_t* pkm, bool v) { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::ST3_SPD(pkm_t* pkm, bool v) { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::ST3_SPE(pkm_t* pkm, bool v) { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::ST3_DEF(pkm_t* pkm, bool v) { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::ST4_1(pkm_t* pkm, bool v)   { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::ST5_1(pkm_t* pkm, bool v)   { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::ST5_2(pkm_t* pkm, bool v)   { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::ST5_3(pkm_t* pkm, bool v)   { Pokemon::ST3(pkm, (Pokemon::ST3(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::ST5_4(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::ST6_1(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::ST6_2(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::ST6_3(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::ST7_1(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::ST7_2(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::ST7_3(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::ST8_1(pkm_t* pkm, bool v)   { Pokemon::ST4(pkm, (Pokemon::ST4(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::RIB0_0(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::RIB0_1(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::RIB0_2(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::RIB0_3(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::RIB0_4(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::RIB0_5(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::RIB0_6(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::RIB0_7(pkm_t* pkm, bool v)  { Pokemon::RIB0(pkm, (Pokemon::RIB0(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::RIB1_0(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::RIB1_1(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::RIB1_2(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::RIB1_3(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::RIB1_4(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::RIB1_5(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::RIB1_6(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::RIB1_7(pkm_t* pkm, bool v)  { Pokemon::RIB1(pkm, (Pokemon::RIB1(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::RIB2_0(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::RIB2_1(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::RIB2_2(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::RIB2_3(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::RIB2_4(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::RIB2_5(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::RIB2_6(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::RIB2_7(pkm_t* pkm, bool v)  { Pokemon::RIB2(pkm, (Pokemon::RIB2(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::RIB3_0(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::RIB3_1(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::RIB3_2(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::RIB3_3(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::RIB3_4(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::RIB3_5(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::RIB3_6(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::RIB3_7(pkm_t* pkm, bool v)  { Pokemon::RIB3(pkm, (Pokemon::RIB3(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::RIB4_0(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::RIB4_1(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::RIB4_2(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::RIB4_3(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::RIB4_4(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::RIB4_5(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::RIB4_6(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::RIB4_7(pkm_t* pkm, bool v)  { Pokemon::RIB4(pkm, (Pokemon::RIB4(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::RIB5_0(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::RIB5_1(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::RIB5_2(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::RIB5_3(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::RIB5_4(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::RIB5_5(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::RIB5_6(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::RIB5_7(pkm_t* pkm, bool v)  { Pokemon::RIB5(pkm, (Pokemon::RIB5(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::_0x36(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x36) = v; }
void Pokemon::_0x37(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x37) = v; }
void Pokemon::contestCount(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x38) = v; }
void Pokemon::battleCount(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x39) = v; }
void Pokemon::dist1(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 0)) | (v ? 1 << 0 : 0)); }
void Pokemon::dist2(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 1)) | (v ? 1 << 1 : 0)); }
void Pokemon::dist3(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 2)) | (v ? 1 << 2 : 0)); }
void Pokemon::dist4(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 3)) | (v ? 1 << 3 : 0)); }
void Pokemon::dist5(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 4)) | (v ? 1 << 4 : 0)); }
void Pokemon::dist6(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 5)) | (v ? 1 << 5 : 0)); }
void Pokemon::dist7(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 6)) | (v ? 1 << 6 : 0)); }
void Pokemon::dist8(pkm_t* pkm, bool v) { Pokemon::dist(pkm, (Pokemon::dist(pkm) & ~(1 << 7)) | (v ? 1 << 7 : 0)); }
void Pokemon::_0x3B(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x3b) = v; }
void Pokemon::_0x3C(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x3c) = v; }
void Pokemon::_0x3D(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x3d) = v; }
void Pokemon::_0x3E(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x3e) = v; }
void Pokemon::_0x3F(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x3f) = v; }

// Region B
void Pokemon::nickname(pkm_t* pkm, u8* v) { memcpy((u8*)(pkm->pk6 + 0x40), v, 0x18); }
void Pokemon::move1(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x5a) = v; }
void Pokemon::move2(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x5c) = v; }
void Pokemon::move3(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x5e) = v; }
void Pokemon::move4(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x60) = v; }
void Pokemon::move1PP(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x62) = v; }
void Pokemon::move2PP(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x63) = v; }
void Pokemon::move3PP(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x64) = v; }
void Pokemon::move4PP(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x65) = v; }
void Pokemon::move1PPPlus(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x66) = v; }
void Pokemon::move2PPPlus(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x67) = v; }
void Pokemon::move3PPPlus(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x68) = v; }
void Pokemon::move4PPPlus(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x69) = v; }
void Pokemon::relearnMove1(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x6a) = v; }
void Pokemon::relearnMove2(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x6c) = v; }
void Pokemon::relearnMove3(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x6e) = v; }
void Pokemon::relearnMove4(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0x70) = v; }
void Pokemon::secretSuperTraining(pkm_t* pkm, bool v) { *(u8*)(pkm->pk6 + 0x72) = (Pokemon::secretSuperTraining(pkm) & ~1) | (v ? 1 : 0); }
void Pokemon::_0x73(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x73) = v; }
void Pokemon::IV_HP(pkm_t* pkm, u8 v)  { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~(0x1f << 00)) | ((v > 31 ? 31 : v) << 00)); }
void Pokemon::IV_ATK(pkm_t* pkm, u8 v) { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~(0x1f << 05)) | ((v > 31 ? 31 : v) << 05)); }
void Pokemon::IV_DEF(pkm_t* pkm, u8 v) { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~(0x1f << 10)) | ((v > 31 ? 31 : v) << 10)); }
void Pokemon::IV_SPE(pkm_t* pkm, u8 v) { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~(0x1f << 15)) | ((v > 31 ? 31 : v) << 15)); }
void Pokemon::IV_SPA(pkm_t* pkm, u8 v) { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~(0x1f << 20)) | ((v > 31 ? 31 : v) << 20)); }
void Pokemon::IV_SPD(pkm_t* pkm, u8 v) { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~(0x1f << 25)) | ((v > 31 ? 31 : v) << 25)); }
void Pokemon::isEgg(pkm_t* pkm, bool v) { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~0x40000000) | (v ? 0x40000000 : v)); }
void Pokemon::isNicknamed(pkm_t* pkm, bool v) { Pokemon::IV32(pkm, (Pokemon::IV32(pkm) & ~0x80000000) | (v ? 0x80000000 : v)); }

// Region C
void Pokemon::HT_name(pkm_t* pkm, u8* v) { memcpy((u8*)(pkm->pk6 + 0x78), v, 0x18); }
void Pokemon::HT_gender(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x92) = v; }
void Pokemon::currentHandler(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x93) = v; }
void Pokemon::geo1Region(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x94) = v; }
void Pokemon::geo1Country(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x95) = v; }
void Pokemon::geo2Region(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x96) = v; }
void Pokemon::geo2Country(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x97) = v; }
void Pokemon::geo3Region(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x98) = v; }
void Pokemon::geo3Country(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x99) = v; }
void Pokemon::geo4Region(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x9a) = v; }
void Pokemon::geo4Country(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x9b) = v; }
void Pokemon::geo5Region(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x9c) = v; }
void Pokemon::geo5Country(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x9d) = v; }
void Pokemon::_0x9E(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x9e) = v; }
void Pokemon::_0x9F(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x9f) = v; }
void Pokemon::_0xA0(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa0) = v; }
void Pokemon::_0xA1(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa1) = v; }
void Pokemon::HT_friendship(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa2) = v; }
void Pokemon::HT_affection(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa3) = v; }
void Pokemon::HT_intensity(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa1) = v; }
void Pokemon::HT_memory(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa5) = v; }
void Pokemon::HT_feeling(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa6) = v; }
void Pokemon::_0xA7(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xa7) = v; }
void Pokemon::HT_textVar(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0xa8) = v; }
void Pokemon::_0xAA(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xaa) = v; }
void Pokemon::_0xAB(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xab) = v; }
void Pokemon::_0xAC(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xac) = v; }
void Pokemon::_0xAD(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xad) = v; }
void Pokemon::fullness(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xae) = v; }
void Pokemon::enjoyment(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xaf) = v; }

// Region D
void Pokemon::OT_name(pkm_t* pkm, u8* v) { memcpy((u8*)(pkm->pk6 + 0xb0), v, 0x18); }
void Pokemon::OT_friendship(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xca) = v; }
void Pokemon::OT_affection(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xcb) = v; }
void Pokemon::OT_intensity(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xcc) = v; }
void Pokemon::OT_memory(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xcd) = v; }
void Pokemon::OT_textVar(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0xce) = v; }
void Pokemon::OT_feeling(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd0) = v; }
void Pokemon::eggYear(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd1) = v; }
void Pokemon::eggMonth(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd2) = v; }
void Pokemon::eggDay(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd3) = v; }
void Pokemon::metYear(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd4) = v; }
void Pokemon::metMonth(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd5) = v; }
void Pokemon::metDay(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd6) = v; }
void Pokemon::_0xD7(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xd7) = v; }
void Pokemon::eggLocation(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0xd8) = v; }
void Pokemon::metLocation(pkm_t* pkm, u16 v) { *(u16*)(pkm->pk6 + 0xda) = v; }
void Pokemon::ball(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xdc) = v; }
void Pokemon::metLevel(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xdd) = (Pokemon::metLevel(pkm) & 0x80) | v; }
void Pokemon::OT_gender(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xdd) = (Pokemon::metLevel(pkm) & ~0x80) | (v << 7); }
void Pokemon::encounterType(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xde) = v; }
void Pokemon::version(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xdf) = v; }
void Pokemon::country(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xe0) = v; }
void Pokemon::region(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xe1) = v; }
void Pokemon::consoleRegion(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xe2) = v; }
void Pokemon::language(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xe3) = v; }

// Battle stats
// void Pokemon::statLevel(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xec) = v; }
// void Pokemon::statCurrentHP(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xf0) = v; }
// void Pokemon::statMaxHP(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xf2) = v; }
// void Pokemon::statATK(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xf4) = v; }
// void Pokemon::statDEF(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xf6) = v; }
// void Pokemon::statSPE(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xf8) = v; }
// void Pokemon::statSPA(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xfa) = v; }
// void Pokemon::statSPD(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0xfc) = v; }

// Abstract values
void Pokemon::markings(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x2a) = v; }
void Pokemon::PKRS(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x2b) = v; }
void Pokemon::ST1(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x2c) = v; }
void Pokemon::ST2(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x2d) = v; }
void Pokemon::ST3(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x2e) = v; }
void Pokemon::ST4(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x2f) = v; }
void Pokemon::RIB0(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x30) = v; }
void Pokemon::RIB1(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x31) = v; }
void Pokemon::RIB2(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x32) = v; }
void Pokemon::RIB3(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x33) = v; }
void Pokemon::RIB4(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x34) = v; }
void Pokemon::RIB5(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x35) = v; }
void Pokemon::dist(pkm_t* pkm, u8 v) { *(u8*)(pkm->pk6 + 0x3a) = v; }
void Pokemon::IV32(pkm_t* pkm, u32 v) { *(u32*)(pkm->pk6 + 0x74) = v; }