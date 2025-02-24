// license:BSD-3-Clause
// copyright-holders:David Haywood
/**************************************************************************************************
  JPM Impact non-video software
**************************************************************************************************/

/* NOTE:
   some games are built on top of other games, and have leftover VFD messages in cases where the
   new machines lacked any VFD display

   in such cases the lamping will clearly be different, and some of the VFD messages
   might be corrupt, where parts of them have been erased.

   see for example:
   Monopoly Bingo (built on top of Big Wheel)
   Reel Money (built on top of Casino Crazy)
   Reel Bingo Classic Club (built on top of Give us a Break Club)
   Wild King Club (slightly corrupt VFD display, placeholder strings)


   ---

   Some games without sound ROMs appear to hang in places waiting for the upd

*/

#include "emu.h"
#include "jpmimpct.h"

// MFME2MAME layouts:
#include "j6aceclb.lh"
#include "j6acehi.lh"
#include "j6amdrm.lh"
#include "j6arcadee.lh"
#include "j6bnkrcl.lh"
#include "j6big50.lh"
#include "j6bigbukc.lh"
#include "j6bigtopa.lh"
#include "j6bnza.lh"
#include "j6btbwd.lh"
#include "j6casclaf.lh"
#include "j6cpal.lh"
#include "j6camelt.lh"
#include "j6cascze.lh"
#include "j6colcsh.lh"
#include "j6crack.lh"
#include "j6drdogh.lh"
#include "j6ewn.lh"
#include "j6euphor.lh"
#include "j6fastfr.lh"
#include "j6ffce.lh"
#include "j6frtpot.lh"
#include "j6gidogh.lh"
#include "j6guabc.lh"
#include "j6guabcl.lh"
#include "j6gldclb.lh"
#include "j6gogold.lh"
#include "j6goldgla.lh"
#include "j6hapyhrb.lh"
#include "j6impuls.lh"
#include "j6impuls15.lh"
#include "j6indy6.lh"
#include "j6indy8.lh"
#include "j6kungfu.lh"
#include "j6kfc.lh"
#include "j6megbck.lh"
#include "j6montlk.lh"
#include "j6montlk6.lh"
#include "j6mono60.lh"
#include "j6mono608.lh"
#include "j6mono6010.lh"
#include "j6monst.lh"
#include "j6oxobin.lh"
#include "j6oxobin10.lh"
#include "j6quick.lh"
#include "j6ra.lh"
#include "j6redarwf.lh"
#include "j6rhchil.lh"
#include "j6rh6cld.lh"
#include "j6reelmn.lh"
#include "j6rico10.lh"
#include "j6roller8.lh"
#include "j6roller10.lh"
#include "j6roller15.lh"
#include "j6snakes.lh"
#include "j6slagng.lh"
#include "j6sonic10.lh"
#include "j6stards.lh"
#include "j6swpdrp.lh"
#include "j6tombc.lh"
#include "j6topflg.lh"
#include "j6twst.lh"
#include "j6untch.lh"
#include "j6vindal.lh"
#include "j6vivark.lh"
#include "j6wldkng.lh"
#include "j6wizardd.lh"



INPUT_PORTS_EXTERN( jpmimpct_inputs );



// note: Apparently the public release of JPeMu v1.45 required the author to specifically encrypt any ROMs
//       before they would run on the emulator, as a way of 'controlling' the users, and what they could run.
//       These are therefore worthless garbage.
//  Football Crazy
//  ROM_LOAD16_BYTE( "fce.p1", 0x00000, 0x020000, CRC(57220618) SHA1(7bd717e438e2bf230179b0f5bb358888a3501c59) )
//  ROM_LOAD16_BYTE( "fce.p2", 0x00001, 0x020000, CRC(16d20bc6) SHA1(0e8ac586ccf3d02189e24bdd2ed88052491aceb6) )
//  j6cas5
//  ROM_LOAD16_BYTE( "c-1.bin", 0x000000, 0x020000, CRC(fc45950a) SHA1(8721758e8f2ac41f26700965ed942cd1a311bb22) )
//  ROM_LOAD16_BYTE( "c-2.bin", 0x000001, 0x020000, CRC(e36aaf42) SHA1(c9da129f85c7b8ce27ea8cb9f090ae647eeac10d) )


// the 68k dies on many sets, but this seems to be due to our emulation, not a problem with the sets
//  the games can be fussy about unmapped reads etc.

/* Non-video Below */

// I believe all IMPACT roms should have a samples rom (it's the only sound output?) so any without are almost
// certainly missing it.



INPUT_PORTS_START( jpmimpct_non_video_inputs ) // these are standard inputs for the first party games
	PORT_INCLUDE( jpmimpct_inputs )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START1 ) // Spin Reels

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_INTERLOCK) PORT_NAME("Back Door")  PORT_CODE(KEYCODE_Q) PORT_TOGGLE

	PORT_MODIFY("J9_2") // are the refill key / cashbox ALWAYS here on non-videos? or just most of the time?
	// J9_2:2 HOPPER ?
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_SERVICE) PORT_NAME("Refill Key") PORT_CODE(KEYCODE_R) PORT_TOGGLE
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_INTERLOCK) PORT_NAME("Cashbox Door")  PORT_CODE(KEYCODE_W) PORT_TOGGLE
INPUT_PORTS_END


// for fixed Jackpot / Stake games that don't have keys for those
INPUT_PORTS_START( j6nokey_withperc )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_MODIFY("J10_0")
	PORT_DIPNAME( 0x01, 0x01, "J10_0: 0x01")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, "J10_0: 0x02")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, "J10_0: 0x04")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "J10_0: 0x08")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, "J10_0: 0x10")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "J10_0: 0x20")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, "J10_0: 0x40")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "J10_0: 0x80")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
INPUT_PORTS_END

// also no % key
INPUT_PORTS_START( j6nokey )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("PERCENT")
	PORT_DIPNAME( 0x01, 0x01, "PERCENT: 0x01")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, "PERCENT: 0x02")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, "PERCENT: 0x04")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "PERCENT: 0x08")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, "PERCENT: 0x10")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "PERCENT: 0x20")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, "PERCENT: 0x40")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "PERCENT: 0x80")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
INPUT_PORTS_END


INPUT_PORTS_START( j6_stake_5_10_20_25 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x50, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "0x30" )
	PORT_CONFSETTING(    0x40, "25p" )
	PORT_CONFSETTING(    0x50, "20p" )
	PORT_CONFSETTING(    0x60, "10p" )
	PORT_CONFSETTING(    0x70, "5p" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_5_10_20_25_30 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x50, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "30p" )
	PORT_CONFSETTING(    0x40, "25p" )
	PORT_CONFSETTING(    0x50, "20p" )
	PORT_CONFSETTING(    0x60, "10p" )
	PORT_CONFSETTING(    0x70, "5p" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_10 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x60, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "0x30" )
	PORT_CONFSETTING(    0x40, "0x40" )
	PORT_CONFSETTING(    0x50, "0x50" )
	PORT_CONFSETTING(    0x60, "10p" )
	PORT_CONFSETTING(    0x70, "0x70" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_25 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x40, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "0x30" )
	PORT_CONFSETTING(    0x40, "25p" )
	PORT_CONFSETTING(    0x50, "0x50" )
	PORT_CONFSETTING(    0x60, "0x60" )
	PORT_CONFSETTING(    0x70, "0x70" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_5_10 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x60, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "0x30" )
	PORT_CONFSETTING(    0x40, "0x40" )
	PORT_CONFSETTING(    0x50, "0x50" )
	PORT_CONFSETTING(    0x60, "10p" )
	PORT_CONFSETTING(    0x70, "5p" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_20_25 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x50, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "0x30" )
	PORT_CONFSETTING(    0x40, "25p" )
	PORT_CONFSETTING(    0x50, "20p" )
	PORT_CONFSETTING(    0x60, "0x60" )
	PORT_CONFSETTING(    0x70, "0x70" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_20_25_30 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x50, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "30p" )
	PORT_CONFSETTING(    0x40, "25p" )
	PORT_CONFSETTING(    0x50, "20p" )
	PORT_CONFSETTING(    0x60, "0x60" )
	PORT_CONFSETTING(    0x70, "0x70" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_20 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x50, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "0x30" )
	PORT_CONFSETTING(    0x40, "0x40" )
	PORT_CONFSETTING(    0x50, "20p" )
	PORT_CONFSETTING(    0x60, "0x60" )
	PORT_CONFSETTING(    0x70, "0x70" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_stake_not_fitted )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x70, 0x70, "Stake Key" )
	PORT_CONFSETTING(    0x00, "0x00" )
	PORT_CONFSETTING(    0x10, "0x10" )
	PORT_CONFSETTING(    0x20, "0x20" )
	PORT_CONFSETTING(    0x30, "0x30" )
	PORT_CONFSETTING(    0x40, "0x40" )
	PORT_CONFSETTING(    0x50, "0x50" )
	PORT_CONFSETTING(    0x60, "0x60" )
	PORT_CONFSETTING(    0x70, "0x70" )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_5 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x07, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "5 GBP" )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_15 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x06, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "15 GBP"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_25 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x05, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "25 GBP"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_8t_10 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x08, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "8 GBP Token"  )
	PORT_CONFSETTING(    0x08, "10 GBP Cash"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_5_15 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x06, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "5 GBP" )
	PORT_CONFSETTING(    0x06, "15 GBP" )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_8_8t_10_15 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x08, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "8 GBP Cash"  )
	PORT_CONFSETTING(    0x09, "8 GBP Token"  )
	PORT_CONFSETTING(    0x08, "10 GBP Cash"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "15 GBP"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_8_8t )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x0a, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "8 GBP Cash"  )
	PORT_CONFSETTING(    0x09, "8 GBP Token"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_8c_5_15 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x06, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "8 GBP Cash"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "5 GBP" )
	PORT_CONFSETTING(    0x06, "15 GBP" )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_not_fitted )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x0f, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_not_fitted_10 ) // where not fitted is valid (but then stake key has no effect)
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x08, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "10 GBP Cash"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_8c )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x0a, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "8 GBP Cash"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "0x08"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END

INPUT_PORTS_START( j6_jackpot_10 )
	PORT_MODIFY("J10_0")
	PORT_CONFNAME( 0x0f, 0x08, "Jackpot / Prize Key" )
	PORT_CONFSETTING(    0x0f, "Not Fitted"  )
	PORT_CONFSETTING(    0x0e, "0x0e"  )
	PORT_CONFSETTING(    0x0d, "0x0d"  )
	PORT_CONFSETTING(    0x0c, "0x0c"  )
	PORT_CONFSETTING(    0x0b, "0x0b"  )
	PORT_CONFSETTING(    0x0a, "0x0a"  )
	PORT_CONFSETTING(    0x09, "0x09"  )
	PORT_CONFSETTING(    0x08, "10 GBP Cash"  )
	PORT_CONFSETTING(    0x07, "0x07"  )
	PORT_CONFSETTING(    0x06, "0x06"  )
	PORT_CONFSETTING(    0x05, "0x05"  )
	PORT_CONFSETTING(    0x04, "0x04"  )
	PORT_CONFSETTING(    0x03, "0x03"  )
	PORT_CONFSETTING(    0x02, "0x02"  )
	PORT_CONFSETTING(    0x01, "0x01"  )
	PORT_CONFSETTING(    0x00, "0x00"  )
INPUT_PORTS_END



INPUT_PORTS_START( j6sonic ) // only runs with 5p or 10p stake and either 8 GBP Token or 10 GBP Cash
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_INCLUDE( j6_jackpot_8t_10 )

	PORT_INCLUDE( j6_stake_5_10 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Lo")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2/Hi")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 )
INPUT_PORTS_END

INPUT_PORTS_START( j6sonicg ) // only runs without a key?
	PORT_INCLUDE( j6sonic )

	PORT_INCLUDE( j6_jackpot_not_fitted )
	PORT_INCLUDE( j6_stake_not_fitted )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Lo")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2/Hi")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
INPUT_PORTS_END

INPUT_PORTS_START( j6arcade )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_INCLUDE( j6_jackpot_5_15 )
	PORT_INCLUDE( j6_stake_20_25_30 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Hold 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Hold 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Hold 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Hopper Dump Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6acehi )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20_25_30 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Hold 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Hold 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Hold 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Hopper Dump Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6big50 )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_INCLUDE( j6_stake_20_25_30 )
	PORT_INCLUDE( j6_jackpot_8_8t_10_15 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Low")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("High")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6bigbuk )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_INCLUDE( j6_stake_5_10_20_25 )
	PORT_INCLUDE( j6_jackpot_8_8t )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Lo")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2/Hi")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Exchange Number")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6bigbnk )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J9_0") // same inputs as j6big50?
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Low")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("High")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Collect")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6bigcsh ) // Empire, non-standard?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_INCLUDE( j6_stake_5_10_20_25_30 )
	PORT_INCLUDE( j6_jackpot_8c_5_15 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6bigpct )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	PORT_INCLUDE( j6_stake_20_25 )
	PORT_INCLUDE( j6_jackpot_not_fitted_10 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) // switch 33
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) // switch 34

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) // switch 41
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) // switch 42
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Lo") // switch 43
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2/Hi") // switch 44
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1") // switch 45
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel") // switch 46

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) // switch 60
INPUT_PORTS_END

INPUT_PORTS_START( j6bigtop )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // for the parent set these must be 'ON' to avoid a NOTE S/W ERROR
	PORT_DIPNAME( 0x01, 0x00, "Stake/Jackpot 1 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "Stake/Jackpot 2 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, "Stake/Jackpot 3 (must be ON)")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "Stake/Jackpot 4 (must be OFF)")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Hold 4/Lo")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Hold 3/Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Hold 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Hold 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END



INPUT_PORTS_START( j6bigwhl )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Wheel Feature 6")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Wheel Feature 5")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Wheel Feature 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Wheel Feature 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Wheel Feature 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Wheel Feature 1")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) // unknown but tested
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) // unknown but tested

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON15 ) // unknown, responds in switch test
INPUT_PORTS_END


INPUT_PORTS_START( j6camelt )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3/Lo")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2/Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Collect")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Blasts")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Cash")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Feature")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Nudges")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6cshbst )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Collect")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("High Left")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("High Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Low Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Low Right")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Cash or Bust")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6cascz )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Bingo")

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Nudges")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Features")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Crazy Spins")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Cash")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON13 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6cascze )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_8c )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Bingo")

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Nudges")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Features")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Crazy Spins")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Cash")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON13 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6caslas )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON6 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6fasttk )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Lo")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Hi")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 )

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6frc10 )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // 2 sets of higher / lower buttons for different parts of the machine
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Higher Right")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Higher Left")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Higher Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Lower Left")

	PORT_MODIFY("J9_0") // exchange / cancel guessed
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON13 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6fbcrz )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Exchange Number")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Go For Goal")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6guab )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // these might be incorrect, layout doesn't seem 100%
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Feature")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Cash")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Bonus")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Knock Out")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Strikes")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Nudges")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("unknown") // responds in test, but no obvious button? not really mapped?

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON15 ) // unknown, responds in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6gogold )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Blasts")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Feature")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Cash")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Nudges")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Cash / Token Sw") // this is named in service mode, is it the same for all games with an input here?
INPUT_PORTS_END

INPUT_PORTS_START( j6hapyhr )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Spin")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6hilosv )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange Number")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Hi-lo Silver Away")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6impuls )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6impls )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // maps to a different place
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
//  PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END


INPUT_PORTS_START( j6impulsa )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6jungfv )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // probably feature related
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Auto Nudge / Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	// PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) NOT tested
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6kungfu )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Lo Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Lo Right")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange Auto Nudge")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	// 0x40 NOT tested
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	// PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) NOT tested
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6mavrk )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange Number")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Lower")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Higher")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Bank Nudges") // guess based on control panel

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6monmad ) // where is the 'take nudges' button?! it even checks bits in PIA ports in the switch check, but it isn't there
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3 / Lower")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2 / Higher")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON9 )
	//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Hopper Dump SW") // NOT tested
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")

	PORT_MODIFY("PIA_PORTB") // some of these register in the switch test!
	PORT_DIPNAME( 0x01, 0x01, "PIA_PORTB: 0x01")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x02, "PIA_PORTB: 0x02")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, "PIA_PORTB: 0x04")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "PIA_PORTB: 0x08")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, "PIA_PORTB: 0x10")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "PIA_PORTB: 0x20")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, "PIA_PORTB: 0x40")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "PIA_PORTB: 0x80")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
INPUT_PORTS_END

INPUT_PORTS_START( j6pog )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Pick Bronze")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Pick Silver")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Pick Gold")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")

	// also reads some bits from the PIA in switch test
INPUT_PORTS_END

INPUT_PORTS_START( j6potg )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Pick Bronze")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Pick Silver")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Pick Gold")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6pwrspn )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Left Higher")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Left Lower")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Right Higher")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Right Lower")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take / Exchange")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON13 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6quick )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange Number")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Cash or Bust")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6reelmn )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	// this one doesn't alarm with bad settings, but instead shows nothing
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("40p Stake")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("60p Stake")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("1GBP Stake")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Exchange")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6richpk )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // seem to pick different trails?
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Pick 3 (Collect Feature)")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Pick 2")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Pick 1 (Collect Nudges)")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Shuffle")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	// PORT_MODIFY("J9_2") // no bits in here show in test mode
INPUT_PORTS_END

INPUT_PORTS_START( j6rico )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2") // unusual mapping with multiple game bits in here
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Top")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Middle")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) // PORT_NAME("Cash / Token Sw")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Bottom")
INPUT_PORTS_END

INPUT_PORTS_START( j6robin )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Shot 3")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Shot 2")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Shot 1")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Play Feature")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Hopper Dump SW") // not tested
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6sidewd )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6snakes )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6stards )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take Cash")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Blasts")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Nudges")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Take")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6strk10 )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Pick 2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Pick 1")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange Nudges")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	// PORT_MODIFY("J9_2") // nothing shown for this port
INPUT_PORTS_END

INPUT_PORTS_START( j6roof )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Swap Trails")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect Feature 1")

	PORT_MODIFY("J9_0")
	// 0x02 isn't tested in service mode
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect Feature 2 / Cancel") // maybe bank nudges?
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Collect")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Nudge 1")

INPUT_PORTS_END

INPUT_PORTS_START( j6wildw )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // other buttons uncertain, possibly a different pair of hi/lo?
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hi")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Lo")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	// 0x40 not tested
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6wizard )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // other buttons uncertain, possibly a different pair of hi/lo?
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Wizard of Odds")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7) PORT_NAME("Exchange Number")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6hikar )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2") // other buttons uncertain, possibly a different pair of hi/lo?
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Wins Hi")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Wins Lo")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Nudge Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Nudge Lo")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Hopper Dump SW")
	// PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON13 ) // not listed
INPUT_PORTS_END

INPUT_PORTS_START( j6slvgst )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON7 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END



INPUT_PORTS_START( j6thril )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_10 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6cshtwr )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Take Feature 3")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take Feature 2")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Feature 1")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Higher")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Lower")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON13 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END




INPUT_PORTS_START( j6cas5 )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Stake")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6cas5e )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_10 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Stake")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6colmon )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Take Cash")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Take Nudge")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Lo Left")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Lo Right")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6crack )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6dyfl )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Lo Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Fire Take")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Exchange For Nudge")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Hi Right")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6firbl )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Lo Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Fire Take")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Exchange For Nudge")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Hi Right")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6firblb )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Lo Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Fire Take")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Exchange For Nudge")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Cancel")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Hi Right")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6fiveln )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Higher")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Lower")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) // PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) // PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6hifly )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Lo Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Xtra Step")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Lo Right")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Press for Roll")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6impact )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Step Down")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Swop")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6hirol )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Stake")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6redarw )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 ) // doesn't care?

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Lo Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Lo Right")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Stake")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Press For Streak")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Exchange For Cash")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Exchange For Feature")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END


INPUT_PORTS_START( j6gforce )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	//PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("H2") // tested, but doesn't exist?

	PORT_MODIFY("J9_1")
	//PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("H3") // tested, but doesn't exist?
INPUT_PORTS_END

INPUT_PORTS_START( j6gforcea )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	//PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("H2") // tested, but doesn't exist?

	PORT_MODIFY("J9_1")
	//PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("H3") // tested, but doesn't exist?
INPUT_PORTS_END



INPUT_PORTS_START( j6hotsht )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Spin Take")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Collect Feature")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Swop Trail")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Hi Button 1")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Lo Button 1")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hi Button 2")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Lo Button 2")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6showtm )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Nudge Exchange")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Collect Feature")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Spin Take")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Nudge Hi")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Nudge Lo")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Cash Hi")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Cash Lo")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6showtmb )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Nudge Exchange")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Collect Feature")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Spin Take")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Nudge Hi")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Nudge Lo")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Cash Hi")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Cash Lo")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6maxod )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Blue Hi")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Yellow Hi")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Green Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Blue Lo")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Yellow Lo")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Green Lo")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON15 ) PORT_NAME("Blue Take")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Yellow Take")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Green Take")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON16 ) PORT_NAME("Learner")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6megbck )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Features")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Feature Knockouts")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Knockouts")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Cash Knockouts")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Cash")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6pinwzd )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Blasts")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Knockout")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Nudges")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6quantm )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Leap")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6ra )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step Down")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6reelth )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Lo Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Lo Right")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6supbrk )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take Breakout")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Cash")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Nudge")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6bags )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Blue Hi")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Yellow Hi")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Green Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Blue Lo")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Yellow Lo")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Green Lo")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON15 ) PORT_NAME("Blue Take")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON14 ) PORT_NAME("Yellow Take")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Green Take")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6tbirds )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Press for Streak")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Take Knockout")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take Bonus")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Nudges")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Cash")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6pompay )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Exchange Feat")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Do or Die")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END



INPUT_PORTS_START( j6vivark )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Press for Streak")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Take Knockout")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take Bonus")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Nudges")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Cash")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6ewn )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Nudge Down")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Nudge Up")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Gamble")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6cpal )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Lo Left")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Lo Right")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Swap")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6easy )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Collect Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Stake")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6r2rum )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Nudges")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step Down")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Hit or Bust")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Lo Left")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Lo Right")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6r2rumc )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Nudges")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Step Down")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Hit or Bust")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Hi Left")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Lo Left")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Hi Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Lo Right")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END


INPUT_PORTS_START( j6slagn )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Cash Pot")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6tqust )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take Dosh Pot")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Dosh")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Bonus")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Feature")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6cshrd )
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_25 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Top Button 3")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Top Button 1")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Top Button 2")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6roller )
	PORT_INCLUDE( j6nokey_withperc ) // stake is always set by dipswitches instead, jackpot hardcoded

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Lo")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2/Hi")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6roller15 )
	PORT_INCLUDE( j6roller )

	PORT_MODIFY("DSW")
	PORT_DIPNAME( 0x03, 0x03, "Stake")
	PORT_DIPSETTING(    0x00, "25p" )
	PORT_DIPSETTING(    0x01, "20p" )
	PORT_DIPSETTING(    0x02, "25p (duplicate)" )
	PORT_DIPSETTING(    0x03, "30p" )
INPUT_PORTS_END

INPUT_PORTS_START( j6roller10 )
	PORT_INCLUDE( j6roller )

	// stake is always 20p?
INPUT_PORTS_END

INPUT_PORTS_START( j6roller8 )
	PORT_INCLUDE( j6roller )

	PORT_MODIFY("DSW")
	PORT_DIPNAME( 0x03, 0x03, "Stake")
	PORT_DIPSETTING(    0x00, "25p" )
	PORT_DIPSETTING(    0x01, "5p" )
	PORT_DIPSETTING(    0x02, "10p" )
	PORT_DIPSETTING(    0x03, "20p" )
INPUT_PORTS_END


INPUT_PORTS_START( j6cdivr ) // gets stuck awarding wins (not payout)
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_15 )
	PORT_INCLUDE( j6_stake_20 )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Top 3")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Top 2")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Top 1")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6grc )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Unknown 2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Unknown 1")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6aceclb )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4/Lo")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	// 80 unused on this game?

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Knockouts")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Super Spins")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Features")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Nudges")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6bnkrcl )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4/Lo")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON7 ) // lights up 2 lamps in service mode

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON8 ) // light up 1 lamp each, probably similar to j6aceclb
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON11 )

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6bnza ) // unusual mapping
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("TNT")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Bullet")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Hold/ Nudge 3/Low")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Hold/ Nudge 2/High")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Hold/Nudge 1")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel/Collect") // 2 buttons, one input?
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6cpclb )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J10_2") // these are guessed
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take 1")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Nudges/Win") // almost the same as cancel / collect?

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4/Lo")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel/Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6casclb )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J10_2") // these are guessed
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Take 4")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Nudges")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6filth )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange") // not sure
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Auto Nudge")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Collect/Cancel")

	PORT_MODIFY("J9_2")
	//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6fivalv )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("unknown")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Shake") // shakes the reels with no sound in one bonus game
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Hey (shake)") // shakes the reels with 'hey' sound in one bonus game
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Collect/Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6guabcl )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J10_2") // these are guessed
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Feature")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Cash")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Bonus")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Knock Out")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("WinSpin")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Nudge")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Take")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6reelb )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange") // guess
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")

	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6indy )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6luckla )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J10_2") // tested but unknown, hangs without sound ROM in places
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4/Lo")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Hi")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel/Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6montlk )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	//PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6mono60 )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6outlaw )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel/Collect")

	//PORT_MODIFY("J9_2") // nothing in here?
INPUT_PORTS_END

INPUT_PORTS_START( j6oxo )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Streak")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")

INPUT_PORTS_END

INPUT_PORTS_START( j6oxobin )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Streak")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6popoli )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6rcclub )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4/Lo")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Hi")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	//PORT_MODIFY("J9_2") // nothing in here?
INPUT_PORTS_END

INPUT_PORTS_START( j6royfls )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Knockouts")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Winspins")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Features")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Nudges")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
INPUT_PORTS_END

INPUT_PORTS_START( j6untch )
	PORT_INCLUDE( j6nokey_withperc )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3/Lo")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2/Hi")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6wldkng )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Gamble") // usually START1
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Bank A Bit")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("unknown") // doesn't light anything in the layout?
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_START1 ) // VERY unusual mapping

	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6h5clb )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0") // buttons unknown, game doesn't use vfd
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel/Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6gldclb )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Step Down")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6raclb )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Step Down")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6tbrdcl_buttons )
	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Streak")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Wins")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Knockouts")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Cash")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel/Collect")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Take Feature")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6tbrdcl ) // test mode broken, but works in clone below?
	PORT_INCLUDE( j6nokey )
	PORT_INCLUDE( j6tbrdcl_buttons )
INPUT_PORTS_END

INPUT_PORTS_START( j6tbrdclg ) // unusual test mode
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first three need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, "DSW 2 (must be ON)")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_INCLUDE( j6tbrdcl_buttons )
INPUT_PORTS_END

INPUT_PORTS_START( j6vivrkc )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Streak")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Take Knockout")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Take Winspins")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Take Feature")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Take Cash")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel/Collect")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END

INPUT_PORTS_START( j6lucklo )
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON13 ) PORT_NAME("Features")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON12 ) PORT_NAME("Knockouts")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON11 ) PORT_NAME("Cash")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON10 ) PORT_NAME("Nudges")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON9 ) PORT_NAME("Winspins")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON8 ) PORT_NAME("Bonus")

	PORT_MODIFY("J9_0")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON7 ) PORT_NAME("Exchange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Collect")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Nudge 4")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel")

	PORT_MODIFY("J9_2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_SERVICE1 ) PORT_NAME("Hopper Dump SW")
	//PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_SERVICE2 ) PORT_NAME("Cash / Token Sw")
INPUT_PORTS_END


//
// MDM games
// these have unusual hookups
//


INPUT_PORTS_START( j6amdrm ) // MDM game - might use % key, errors when changed from previous boot, but doesn't seem to require one
	PORT_INCLUDE( jpmimpct_inputs )

	PORT_MODIFY("J10_0") // the inputs connect where the jackpot / stake key would usually go!
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START1 ) // Spin Reels
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Exhcange")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3 / Lo")
	PORT_DIPNAME( 0x08, 0x08, "J10_0: 3")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, "J10_0: 4")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "J10_0: 5")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, "J10_0: 6")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "J10_0: 7")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_INTERLOCK) PORT_NAME("Cashbox Door")  PORT_CODE(KEYCODE_W) PORT_TOGGLE
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_INTERLOCK) PORT_NAME("Back Door")  PORT_CODE(KEYCODE_Q) PORT_TOGGLE
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_SERVICE) PORT_NAME("Refill Key") PORT_CODE(KEYCODE_R) PORT_TOGGLE

	PORT_MODIFY("J10_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2 / Hi")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_NAME("Fast Track")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")
INPUT_PORTS_END

INPUT_PORTS_START( j6col ) // might use % key, errors when changed from previous boot, but doesn't seem to require one
	PORT_INCLUDE( jpmimpct_inputs )

	PORT_MODIFY("J10_0") // the inputs connect where the jackpot / stake key would usually go!
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_START1 ) // Spin Reels
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_NAME("Exhcange")
	PORT_DIPNAME( 0x04, 0x04, "J10_0: 2")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, "J10_0: 3")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x10, 0x10, "J10_0: 4")
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, "J10_0: 5")
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, "J10_0: 6")
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, "J10_0: 7")
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_MODIFY("J10_2")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_INTERLOCK) PORT_NAME("Cashbox Door")  PORT_CODE(KEYCODE_W) PORT_TOGGLE
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_INTERLOCK) PORT_NAME("Back Door")  PORT_CODE(KEYCODE_Q) PORT_TOGGLE
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_SERVICE) PORT_NAME("Refill Key") PORT_CODE(KEYCODE_R) PORT_TOGGLE

	PORT_MODIFY("J10_1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_NAME("Nudge 3")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_NAME("Nudge 2")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_NAME("Nudge 1")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_NAME("Cancel / Collect")
INPUT_PORTS_END

//
// Empire games
//

INPUT_PORTS_START( j6papa ) // Empire
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6wthing ) // Empire
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END




//
// JPM / Crystal / Ace games with other issues
// The following games won't enter test mode properly, why? different method? or similar to previous input issues?
//

INPUT_PORTS_START( j6colcsh ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// this allows for more complex mixes, for now just use a setting that boots
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )

INPUT_PORTS_END

INPUT_PORTS_START( j6cshvgs ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6cheque ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6dmngz ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6euphor ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6euphorf ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6fireck ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_10 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6rccls ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END


INPUT_PORTS_START( j6firclb ) // can't get to input test?
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first two need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6crzclb ) // can't get to input test?
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first two need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6cluclb ) // can't get to input test?
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first three need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, "DSW 2 (must be ON)")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END


INPUT_PORTS_START( j6framft ) // can't get to input test?
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first two need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6frtmch ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_20 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6gidogh ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6goldgl ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6hiphop ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6pogcls ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_5 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6rh6 ) // can't get to input test?
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first three need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, "DSW 2 (must be ON)")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6rhchil ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6spcinv ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6swpdrp ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6tbirdsk ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6tomb ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6topflg ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6twst ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6vindal ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6knight ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6btbw ) // can't get to input test?
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first three need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, "DSW 2 (must be ON)")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END


INPUT_PORTS_START( j6colic ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6crakr ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6kapang ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6pinfvr ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6shoot ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6bucks ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6scarlt ) // can't get to input test?
	PORT_INCLUDE( jpmimpct_non_video_inputs )

	// TODO: more possibilities than this, this just allows it to boot
	PORT_INCLUDE( j6_jackpot_25 )
	PORT_INCLUDE( j6_stake_25 )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6rh6cl ) // can't get to input test?
	PORT_INCLUDE( j6nokey )

	PORT_MODIFY("DSW") // first three need to be on to avoid error 91 00 (invalid mode) rather than the usual key ones
	PORT_DIPNAME( 0x01, 0x00, "DSW 0 (must be ON)")
	PORT_DIPSETTING(    0x01, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "DSW 1 (must be ON)")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x00, "DSW 2 (must be ON)")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6cccla ) // can't get to input test?
	PORT_INCLUDE( j6nokey )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6cascla ) // can't get to input test?
	PORT_INCLUDE( j6nokey )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6magcir ) // can't get to input test?
	PORT_INCLUDE( j6nokey )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6ramese ) // can't get to input test?
	PORT_INCLUDE( j6nokey )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6hirlcl ) // can't get to input test?
	PORT_INCLUDE( j6nokey )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END

INPUT_PORTS_START( j6start ) // not a standard machine, has a crane
	PORT_INCLUDE( j6nokey_withperc )
	// PORT_INCLUDE( TODO: )  // not verified
INPUT_PORTS_END


ROM_START( j6fifth )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fdm30dsk.1", 0x000000, 0x020000, CRC(ec6a2687) SHA1(4cafd1c8d6d20fb034493c16d3abfafa2a1906f5) )
	ROM_LOAD16_BYTE( "fdm30.2",    0x000001, 0x020000, CRC(b24995cc) SHA1(3ee23e0e8e805c077c2c60c463d9d0214c61fc2d) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END


ROM_START( j6aceclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "aocd1.bin", 0x000000, 0x020000, CRC(932fe07a) SHA1(4f4dbfa627364edf8408264e547d0f21d306c59e) )
	ROM_LOAD16_BYTE( "aocd2.bin", 0x000001, 0x020000, CRC(d18d8b24) SHA1(df09419c3702efb340da3e7327e1139c54351926) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "aceofclubssnd.bin", 0x000000, 0x080000, CRC(9ea8bce4) SHA1(0849be87528168dbc5c07a31138edcb30a611c5c) )
ROM_END

ROM_START( j6aceclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "aocdp1.b8", 0x000000, 0x020000, CRC(78b1b442) SHA1(b2fe66e3ac5b7a2eb4b0dbcc6237259aef61abfd) )
	ROM_LOAD16_BYTE( "aocd2.bin", 0x000001, 0x020000, CRC(d18d8b24) SHA1(df09419c3702efb340da3e7327e1139c54351926) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "aceofclubssnd.bin", 0x000000, 0x080000, CRC(9ea8bce4) SHA1(0849be87528168dbc5c07a31138edcb30a611c5c) )
ROM_END




ROM_START( j6acehi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "aceshigh.p1", 0x000000, 0x020000, CRC(13c48483) SHA1(43e6a9a00ee9c128700f763a18451abb5634e78f) )
	ROM_LOAD16_BYTE( "aceshigh.p2", 0x000001, 0x020000, CRC(21d2b908) SHA1(b6fc67d2d5f79e21be154d8f84c54f0e4a0b702c) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "aceshighsnd.bin", 0x000000, 0x080000, CRC(d0d8ec93) SHA1(3731c64c194fe856f0bebe5b1b4430bd82ee905f) )
ROM_END


ROM_START( j6amdrm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "americandreamgame1.bin", 0x000000, 0x020000, CRC(03006f4d) SHA1(f2da5d3bc1cb76380f8fc1182e48d3906f0c23b8) )
	ROM_LOAD16_BYTE( "americandreamgame2.bin", 0x000001, 0x020000, CRC(ae336948) SHA1(f620b6098a488c613fd2468f99b5b80347aaea9e) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "americandreamsound.bin", 0x000000, 0x080000, CRC(8a23d027) SHA1(c475032411e28c4c889a3c112ca332ba222b93fe) )
ROM_END

#define j6arcade_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "arca-snd.bin", 0x000000, 0x080000, CRC(111c3c40) SHA1(9ce6da8101eb9c26c7ff5616ef24e7c119803777) )

ROM_START( j6arcade )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca-9_1.bin", 0x000000, 0x020000, CRC(09838350) SHA1(ffb37a229e20fd2f9690659bc4a2841269b5d918) )
	ROM_LOAD16_BYTE( "arca-9_2.bin", 0x000001, 0x020000, CRC(a2170a26) SHA1(ed23f9a4b197c1603014eff238c73564fca1bd0c) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadea )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca-9a1.bin", 0x000000, 0x020000, CRC(5b474331) SHA1(a8cfadab98ece97fb66fcf2e87e97be528ab5bf7) )
	ROM_LOAD16_BYTE( "arca-9_2.bin", 0x000001, 0x020000, CRC(a2170a26) SHA1(ed23f9a4b197c1603014eff238c73564fca1bd0c) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadeb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca-9n1.bin", 0x000000, 0x020000, CRC(38b53b94) SHA1(a28d7ac1eb594251d9534901ee38b797cd563dc7) )
	ROM_LOAD16_BYTE( "arca-9_2.bin", 0x000001, 0x020000, CRC(a2170a26) SHA1(ed23f9a4b197c1603014eff238c73564fca1bd0c) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadec )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca-9np.bin", 0x000000, 0x020000, CRC(d32b6fac) SHA1(1cb16c214da888adacba090d59f88694d1d9e25b) )
	ROM_LOAD16_BYTE( "arca-9_2.bin", 0x000001, 0x020000, CRC(a2170a26) SHA1(ed23f9a4b197c1603014eff238c73564fca1bd0c) )
	j6arcade_sound
ROM_END

ROM_START( j6arcaded )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca-9p1.bin", 0x000000, 0x020000, CRC(e21dd768) SHA1(94132c7ba9b90ab6b52a56e60b68bf4e5d30e063) )
	ROM_LOAD16_BYTE( "arca-9_2.bin", 0x000001, 0x020000, CRC(a2170a26) SHA1(ed23f9a4b197c1603014eff238c73564fca1bd0c) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadee )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca10_1.bin", 0x000000, 0x020000, CRC(4c632e9e) SHA1(1e76e26941b164ba3a51c1c3caaa3b4d384a90d3) )
	ROM_LOAD16_BYTE( "arca10_2.bin", 0x000001, 0x020000, CRC(2175520c) SHA1(0a12506a72a93c8cd74f6666d41bacdfd4e72a54) )
	j6arcade_sound
ROM_END

// this differs by 2 bytes from arca10_2.bin, and fails checksum test
//ROM_LOAD16_BYTE( "arcadia.p2",   0x000001, 0x020000, CRC(1533ea6f) SHA1(0dff53bcee961781312eb108cd705664f772ce1d) )

ROM_START( j6arcadef )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca10a1.bin", 0x000000, 0x020000, CRC(1ea7eeff) SHA1(8b8a1b0543d53d95fd2fd44add1114c6ad48b6c7) )
	ROM_LOAD16_BYTE( "arca10_2.bin", 0x000001, 0x020000, CRC(2175520c) SHA1(0a12506a72a93c8cd74f6666d41bacdfd4e72a54) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadeg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca10n1.bin", 0x000000, 0x020000, CRC(7d55965a) SHA1(96ca11708bbba84ccbb0c43b6ddaca8c9285ffb8) )
	ROM_LOAD16_BYTE( "arca10_2.bin", 0x000001, 0x020000, CRC(2175520c) SHA1(0a12506a72a93c8cd74f6666d41bacdfd4e72a54) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadeh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca10np.bin", 0x000000, 0x020000, CRC(96cbc262) SHA1(0fdb783443240db94bf22a09f2177de958a2ecfe) )
	ROM_LOAD16_BYTE( "arca10_2.bin", 0x000001, 0x020000, CRC(2175520c) SHA1(0a12506a72a93c8cd74f6666d41bacdfd4e72a54) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadei )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "arca10p1.bin", 0x000000, 0x020000, CRC(a7fd7aa6) SHA1(9f083900ee50d1669f23eecc7d6d7779d2a27301) )
	ROM_LOAD16_BYTE( "arca10_2.bin", 0x000001, 0x020000, CRC(2175520c) SHA1(0a12506a72a93c8cd74f6666d41bacdfd4e72a54) )
	j6arcade_sound
ROM_END

ROM_START( j6arcadej )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF ) // overdumps? might be useful to verify which p2 is correct tho
	ROM_LOAD16_BYTE( "arcadiaarca10-1.bin", 0x000000, 0x080000, CRC(998ae103) SHA1(cabbd7ec8dcaf5107de0c23a8d67680254acbe5d) )
	ROM_LOAD16_BYTE( "arcadiaarca10-2.bin", 0x000001, 0x080000, CRC(98c15e57) SHA1(3207c9760ca1b9f11b24253d6c974e4efeb0c46c) )
	j6arcade_sound
ROM_END


#define j6bnkrcl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cbsnd.bin", 0x000000, 0x080000, CRC(e7e587c9) SHA1(fde7a7761253dc4133340b766d220873731c11c7) )
ROM_START( j6bnkrcl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clba-6_1.bin", 0x000000, 0x020000, CRC(b0407200) SHA1(88792a7cc71be3830a0156fa10195bad3cc58066) )
	ROM_LOAD16_BYTE( "clba-6_2.bin", 0x000001, 0x020000, CRC(fd055187) SHA1(7634c1dfbfab66c13c2a0453dc70643f9b33edef) )
	j6bnkrcl_sound
ROM_END

ROM_START( j6bnkrcla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clba-6f1.bin", 0x000000, 0x020000, CRC(749a9110) SHA1(8607d6edd4d2f2d799d3a60ead779f9a80e93823) )
	ROM_LOAD16_BYTE( "clba-6_2.bin", 0x000001, 0x020000, CRC(fd055187) SHA1(7634c1dfbfab66c13c2a0453dc70643f9b33edef) )
	j6bnkrcl_sound
ROM_END

ROM_START( j6bnkrclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clba-6p1.bin", 0x000000, 0x020000, CRC(5bde2638) SHA1(d7247d74acb030c986aecc58991c56207ae2ade9) )
	ROM_LOAD16_BYTE( "clba-6_2.bin", 0x000001, 0x020000, CRC(fd055187) SHA1(7634c1dfbfab66c13c2a0453dc70643f9b33edef) )
	j6bnkrcl_sound
ROM_END

ROM_START( j6bnkrclc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cl_banke.p1", 0x00000, 0x020000, CRC(4604d13d) SHA1(8e96b5ac7537fbe496178f26be0786049efcbc49) )
	ROM_LOAD16_BYTE( "cl_banke.p2", 0x00001, 0x020000, CRC(6aea1ffd) SHA1(cf9356106f5e64ebfde6d98314afad3a5377eaf4) )
	j6bnkrcl_sound
ROM_END


#define j6big50_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1386.bin", 0x000000, 0x080000, CRC(72ddabc9) SHA1(c68b0896b4c25b591029231dff045b16eab61ac4) )

ROM_START( j6big50 )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9347.bin", 0x000000, 0x020000, CRC(2d05fc2b) SHA1(c5c3cf89b5d75876eecc9f9acf0426b58bacbd79) )
	ROM_LOAD16_BYTE( "9348.bin", 0x000001, 0x020000, CRC(d131967e) SHA1(9028c3d55d365de9e90ea4a2af29aaace0d8e588) )
	j6big50_sound
ROM_END

ROM_START( j6big50a )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9349.bin", 0x000000, 0x020000, CRC(c69ba813) SHA1(6b12aac1ed5bfdb71cc6ece67162fc890ec924a6) )
	ROM_LOAD16_BYTE( "9348.bin", 0x000001, 0x020000, CRC(d131967e) SHA1(9028c3d55d365de9e90ea4a2af29aaace0d8e588) )
	j6big50_sound
ROM_END

ROM_START( j6big50b )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9350.bin", 0x000000, 0x020000, CRC(7fc13c4a) SHA1(a260914f04ea05ac9e9fbf53d914f54a3e84b3ee) )
	ROM_LOAD16_BYTE( "9348.bin", 0x000001, 0x020000, CRC(d131967e) SHA1(9028c3d55d365de9e90ea4a2af29aaace0d8e588) )
	j6big50_sound
ROM_END

ROM_START( j6big50c )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9351.bin", 0x000000, 0x020000, CRC(4f688da3) SHA1(0abb33b3783a73ddd5f1677bf84075682da09bcf) )
	ROM_LOAD16_BYTE( "9348.bin", 0x000001, 0x020000, CRC(d131967e) SHA1(9028c3d55d365de9e90ea4a2af29aaace0d8e588) )
	j6big50_sound
ROM_END

ROM_START( j6big50d )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bg50v8p1", 0x000000, 0x020000, CRC(a28eadba) SHA1(fb7bb3305324a80454c1b4fa053c185d721e387e) )
	ROM_LOAD16_BYTE( "bg50v8p2", 0x000001, 0x020000, CRC(6724f61d) SHA1(9ccd7940c934f1edbba7c7defc87f8ac12ed5374) )
	j6big50_sound
ROM_END

#define j6bigbnk_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1109.bin", 0x000000, 0x080000, CRC(b4d7ac12) SHA1(ac194d15e9d4e5cdadddbf2dc3c9660b52f116c2) )

ROM_START( j6bigbnk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20346.bin", 0x000000, 0x020000, CRC(6e717ecd) SHA1(f285a2d0fb0aa56b743a922087c416fa66ec1f52) )
	ROM_LOAD16_BYTE( "20347.bin", 0x000001, 0x020000, CRC(c515e34e) SHA1(fc3aeb2f3fa9ba463f8f39cf26d3795be869ffbc) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20348.bin", 0x000000, 0x020000, CRC(85ef2af5) SHA1(ccfd4e5296e3889045b0b8ba11a066104b266f13) )
	ROM_LOAD16_BYTE( "20347.bin", 0x000001, 0x020000, CRC(c515e34e) SHA1(fc3aeb2f3fa9ba463f8f39cf26d3795be869ffbc) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20349.bin", 0x000000, 0x020000, CRC(3cb5beac) SHA1(017fe4f39faff27e23ef1eba7dd687a276824da7) )
	ROM_LOAD16_BYTE( "20347.bin", 0x000001, 0x020000, CRC(c515e34e) SHA1(fc3aeb2f3fa9ba463f8f39cf26d3795be869ffbc) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20350.bin", 0x000000, 0x020000, CRC(0c1c0f45) SHA1(6da230716fd6bcb2df55857cf8bfa1d3a6fd0484) )
	ROM_LOAD16_BYTE( "20347.bin", 0x000001, 0x020000, CRC(c515e34e) SHA1(fc3aeb2f3fa9ba463f8f39cf26d3795be869ffbc) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7660.bin", 0x000000, 0x020000, CRC(af560c17) SHA1(1a3b8db230d5ade8dbdd2f1d776c54d1c3601992) )
	ROM_LOAD16_BYTE( "7661.bin", 0x000001, 0x020000, CRC(d0365768) SHA1(29d11dbeb5ffbd692247a42a9f14668cb376d56d) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnke )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7662.bin", 0x000000, 0x020000, CRC(44c8582f) SHA1(e877134bfd9b3ac638340f9e0e02f3f63d52c434) )
	ROM_LOAD16_BYTE( "7661.bin", 0x000001, 0x020000, CRC(d0365768) SHA1(29d11dbeb5ffbd692247a42a9f14668cb376d56d) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7663.bin", 0x000000, 0x020000, CRC(fd92cc76) SHA1(f566a1c6fdbbee6db5efcb0eef8bce39d48a4cf2) )
	ROM_LOAD16_BYTE( "7661.bin", 0x000001, 0x020000, CRC(d0365768) SHA1(29d11dbeb5ffbd692247a42a9f14668cb376d56d) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7426.bin", 0x000000, 0x020000, CRC(d8cbee6c) SHA1(47a86689a8dc2946765520633d3d1f0fb4a2a051) )
	ROM_LOAD16_BYTE( "7427.bin", 0x000001, 0x020000, CRC(ceab9d77) SHA1(56b56781983509db84eeb53c2f70010d1d221fff) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7428.bin", 0x000000, 0x020000, CRC(3355ba54) SHA1(48eb078d0ec0b56a997fa949a2054da1fd24b1f2) )
	ROM_LOAD16_BYTE( "7427.bin", 0x000001, 0x020000, CRC(ceab9d77) SHA1(56b56781983509db84eeb53c2f70010d1d221fff) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnki )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6996.bin", 0x000000, 0x020000, CRC(5fcf0a19) SHA1(07ca6d27db65b7af16f330e5fb284f7b72220a23) )
	ROM_LOAD16_BYTE( "6997.bin", 0x000001, 0x020000, CRC(ef4bb8ba) SHA1(1fdbffbc5e504934310cca90eb6f2ca4194eea77) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6998.bin", 0x000000, 0x020000, CRC(b4515e21) SHA1(b8fc75056b4e5be288a6a7e210c11d9ea6c7b48a) )
	ROM_LOAD16_BYTE( "6997.bin", 0x000001, 0x020000, CRC(ef4bb8ba) SHA1(1fdbffbc5e504934310cca90eb6f2ca4194eea77) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6999.bin", 0x000000, 0x020000, CRC(0d0bca78) SHA1(8b28af82f4f21d4e7102a9787aa54413ed46e21c) )
	ROM_LOAD16_BYTE( "6997.bin", 0x000001, 0x020000, CRC(ef4bb8ba) SHA1(1fdbffbc5e504934310cca90eb6f2ca4194eea77) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7000.bin", 0x000000, 0x020000, CRC(5382a428) SHA1(86f513cbd969dbfbcac53de637e3796daab03037) )
	ROM_LOAD16_BYTE( "6997.bin", 0x000001, 0x020000, CRC(ef4bb8ba) SHA1(1fdbffbc5e504934310cca90eb6f2ca4194eea77) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bb20p1ac", 0x000000, 0x020000, CRC(9865f664) SHA1(e44263ade7dbe4c4e0d0616afd6502780b058d8f) )
	ROM_LOAD16_BYTE( "bb20p2ac", 0x000001, 0x020000, CRC(16d40b7e) SHA1(df81916860495f6d53ed4016c435dde8d6d7ad5f) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bba915p1", 0x000000, 0x020000, CRC(3b8ccfbc) SHA1(7f42d9dbcf976c12721324ea29cf942ad634985d) )
	ROM_LOAD16_BYTE( "bba915p2", 0x000001, 0x020000, CRC(bad068d5) SHA1(dd77602f7bdbcb04c7e6d3cb4a52e270fd3e7d52) )
	j6bigbnk_sound
ROM_END


ROM_START( j6bigbnko ) // are the numbers correct?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7373.bin", 0x000000, 0x020000, CRC(36c4b97c) SHA1(e925c85220ff96af5dde67300f7c0b9e3e54dcb2) )
	ROM_LOAD16_BYTE( "7306.bin", 0x000001, 0x020000, CRC(b33ec3b3) SHA1(12abb4621b9bce57994c1958c9df66b1048d8819) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bigbnkp )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bbv4p18c", 0x000000, 0x020000, CRC(793d3d36) SHA1(d62e2ee62ed4f3a19c05edae1ea092679a57e8a5) ) // aka bb841ac
	ROM_LOAD16_BYTE( "bbv48p2",  0x000001, 0x020000, CRC(96a9f1f5) SHA1(2b8b4f06edbe35f5b73d25bac8a973feeaa77b47) )
	j6bigbnk_sound
ROM_END

ROM_START( j6bbankr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "bigbanker-crystal-p1.bin", 0x00001, 0x020000, CRC(3f246acd) SHA1(bd6813d1477da9ee8e7c289123ba3aed5b3bb076) ) // aka bb21.bin
	ROM_LOAD16_BYTE(  "bigbanker-crystal-p2.bin", 0x00000, 0x020000, CRC(313ef240) SHA1(aa88ddb383c4c292ab610854407665842fedd374) ) // aka bb22.bin
	j6bigbnk_sound
ROM_END

#define j6bigbuk_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "bb_snd.bin", 0x000000, 0x080000, CRC(d4d57f9f) SHA1(2ec38b62928d8c208880015b3a5e348e9b1c2079) )

ROM_START( j6bigbuk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7479.bin", 0x000000, 0x020000, CRC(a70145c7) SHA1(8da5b84c1842071b2273381f3d88d5bea7794ca1) )
	ROM_LOAD16_BYTE( "7480.bin", 0x000001, 0x020000, CRC(6fa0b3b0) SHA1(dde0e133e5efd3ebb245da4e51e9c8ca91374659) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbuka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7481.bin", 0x000000, 0x020000, CRC(4c9f11ff) SHA1(20e05cfd2a7166becd58711d48d12fdd90877953) )
	ROM_LOAD16_BYTE( "7480.bin", 0x000001, 0x020000, CRC(6fa0b3b0) SHA1(dde0e133e5efd3ebb245da4e51e9c8ca91374659) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbukb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7482.bin", 0x000000, 0x020000, CRC(f5c585a6) SHA1(7c080e9113e1b7eedd3bfd1eb7096591943bdd43) )
	ROM_LOAD16_BYTE( "7480.bin", 0x000001, 0x020000, CRC(6fa0b3b0) SHA1(dde0e133e5efd3ebb245da4e51e9c8ca91374659) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbukc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9478.bin", 0x000000, 0x020000, CRC(28f6194b) SHA1(e4ecf64eed37812b86f52aa06417594e780982d6) )
	ROM_LOAD16_BYTE( "9479.bin", 0x000001, 0x020000, CRC(194010ff) SHA1(abf4b6fd1cd97fd4352d57b5d85188b78be13887) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbukd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9480.bin", 0x000000, 0x020000, CRC(c3684d73) SHA1(dab582b30104bd581e5c11c574d38b985182605e) )
	ROM_LOAD16_BYTE( "9479.bin", 0x000001, 0x020000, CRC(194010ff) SHA1(abf4b6fd1cd97fd4352d57b5d85188b78be13887) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbuke )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9481.bin", 0x000000, 0x020000, CRC(7a32d92a) SHA1(e4e1d9c95373e0c0a28f6e1886441ebdb57ca1bb) )
	ROM_LOAD16_BYTE( "9479.bin", 0x000001, 0x020000, CRC(194010ff) SHA1(abf4b6fd1cd97fd4352d57b5d85188b78be13887) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbukf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9482.bin", 0x000000, 0x020000, CRC(4a9b68c3) SHA1(69a475efb305722f6a0bf7165d32396de104bb82) )
	ROM_LOAD16_BYTE( "9479.bin", 0x000001, 0x020000, CRC(194010ff) SHA1(abf4b6fd1cd97fd4352d57b5d85188b78be13887) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbukg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bb101.hex", 0x000000, 0x020000, CRC(d38e13bd) SHA1(1665160ad34d693774145b44da751c36c5b316b8) )
	ROM_LOAD16_BYTE( "bb102.hex", 0x000001, 0x020000, CRC(3772ae99) SHA1(68d6a8ffc35568742952811a698da465af0e6925) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbukh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bbuv8p1", 0x000000, 0x020000, CRC(0db84f74) SHA1(2e568576ee33c5ad7bc7e813c7e5598f5bd82dd9) )
	ROM_LOAD16_BYTE( "bbuv8p2", 0x000001, 0x020000, CRC(4b016cf7) SHA1(94a846c92a5ac39fd9f734de833e7dfeba031ee4) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbuki )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bibuv8p1", 0x000000, 0x020000, CRC(5f7c8f15) SHA1(fa40783a7f8731ccc279dff723f554a7170a41d7) )
	ROM_LOAD16_BYTE( "bbuv8p2", 0x000001, 0x020000, CRC(4b016cf7) SHA1(94a846c92a5ac39fd9f734de833e7dfeba031ee4) )
	j6bigbuk_sound
ROM_END

ROM_START( j6bigbukj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bibv58p1", 0x000000, 0x020000, CRC(3a89174d) SHA1(a356bcc6421d185ba8403da6fffda384a929676b) )
	ROM_LOAD16_BYTE( "bbuv58p2", 0x000001, 0x020000, NO_DUMP )
	j6bigbuk_sound
ROM_END




ROM_START( j6bigcsh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bcm4_2.p1", 0x000000, 0x020000, CRC(f412dcf8) SHA1(5e008b1966bd1aed3173836bbd2b09e6b368ac52) )
	ROM_LOAD16_BYTE( "bcm4_2.p2", 0x000001, 0x020000, CRC(edc32567) SHA1(5e1682399ac7d6f06d63c840c79d7ead6c0e4bd6) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "bcmsnd.bin", 0x000000, 0x080000, CRC(4acd8905) SHA1(49c19a25fd6a7bdddc6a3d8bed663019fc6b0ccc) )
ROM_END

#define j6bigpct_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "bigpicture_sound", 0x000000, 0x080000, NO_DUMP )

ROM_START( j6bigpct )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "big11nsk.1", 0x000000, 0x020000, CRC(fabe2c0d) SHA1(522cd19e6e947afb485e6fd81e3589a97ec5ba0b) )
	ROM_LOAD16_BYTE( "big11.2",    0x000001, 0x020000, CRC(34c3695e) SHA1(c3ab8710ebdc4d5f368d5b2a0c4803e939bc8bd8) )
	j6bigpct_sound
ROM_END

ROM_START( j6bigpcta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "big11dsk.1", 0x000000, 0x020000, CRC(11207835) SHA1(6e175d6fbe27446b058f885ae1a1ca98dc3ef409) )
	ROM_LOAD16_BYTE( "big11.2",    0x000001, 0x020000, CRC(34c3695e) SHA1(c3ab8710ebdc4d5f368d5b2a0c4803e939bc8bd8) )
	j6bigpct_sound
ROM_END

ROM_START( j6bigpctb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "big11nak.1", 0x000000, 0x020000, CRC(a87aec6c) SHA1(1893f89e673a05926d20f6b6c318af09859d8f7d) )
	ROM_LOAD16_BYTE( "big11.2",    0x000001, 0x020000, CRC(34c3695e) SHA1(c3ab8710ebdc4d5f368d5b2a0c4803e939bc8bd8) )
	j6bigpct_sound
ROM_END


#define j6bigtop_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "btcl-snd.bin", 0x000000, 0x080000, CRC(160d0317) SHA1(bb111b0a96fde85acd197ef9147eae2b7059da36) )

ROM_START( j6bigtop )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20870.bin", 0x000000, 0x020000, CRC(3a9d8758) SHA1(0736eefb516f104272cc6269f1850b5f03b6186d) )
	ROM_LOAD16_BYTE( "20868.bin", 0x000001, 0x020000, CRC(92fce54d) SHA1(4396ad175fae258fd00f7dc362c36d6065b4bfb4) )
	j6bigtop_sound
ROM_END

ROM_START( j6bigtopa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "btcl-4s1.bin", 0x000000, 0x020000, CRC(9678b0c2) SHA1(e20c778935f0c710444230d6f06d6572c976d5dd) )
	ROM_LOAD16_BYTE( "btcl-4s2.bin", 0x000001, 0x020000, CRC(25a38540) SHA1(ee1cabb62f998e43f10a0c34bdca916ca2f1b01c) )
	j6bigtop_sound
ROM_END

ROM_START( j6bigtopb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "btcl-4p1.bin", 0x000000, 0x020000, CRC(7de6e4fa) SHA1(c709d3d155fa689ea53f6839a0900b1c3f452a66) )
	ROM_LOAD16_BYTE( "btcl-4s2.bin", 0x000001, 0x020000, CRC(25a38540) SHA1(ee1cabb62f998e43f10a0c34bdca916ca2f1b01c) )
	j6bigtop_sound
ROM_END

ROM_START( j6bigtopc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "btcl-4f1.bin", 0x000000, 0x020000, CRC(52a253d2) SHA1(bd518da4d9daab3dfcd26f723e907bca16576e63) )
	ROM_LOAD16_BYTE( "btcl-4s2.bin", 0x000001, 0x020000, CRC(25a38540) SHA1(ee1cabb62f998e43f10a0c34bdca916ca2f1b01c) )
	j6bigtop_sound
ROM_END


#define j6bigwhl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "bigwheelsnd.bin", 0x000000, 0x080000, CRC(90a19aaa) SHA1(7b17e9fda01d4fb163e09107759a6bf473fc6dc0) )

ROM_START( j6bigwhl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9538.bin", 0x000000, 0x020000, CRC(4a3fee6e) SHA1(3a55a1c89a562877f9db805bd036d7566c2cb4a2) )
	ROM_LOAD16_BYTE( "9539.bin", 0x000001, 0x020000, CRC(98335286) SHA1(57792f661e82cbb7afe6e1723a419219c7b6e7b7) )
	j6bigwhl_sound
ROM_END

ROM_START( j6bigwhla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9540.bin", 0x000000, 0x020000, CRC(a1a1ba56) SHA1(1ac886e66182d86797495467edfb099bba8f03be) )
	ROM_LOAD16_BYTE( "9539.bin", 0x000001, 0x020000, CRC(98335286) SHA1(57792f661e82cbb7afe6e1723a419219c7b6e7b7) )
	j6bigwhl_sound
ROM_END

ROM_START( j6bigwhlb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9541.bin", 0x000000, 0x020000, CRC(18fb2e0f) SHA1(fa8f348cbd465f6ba9c74144cfbeb74b31f57be0) )
	ROM_LOAD16_BYTE( "9539.bin", 0x000001, 0x020000, CRC(98335286) SHA1(57792f661e82cbb7afe6e1723a419219c7b6e7b7) )
	j6bigwhl_sound
ROM_END

ROM_START( j6bigwhlc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9542.bin", 0x000000, 0x020000, CRC(28529fe6) SHA1(2b110dd8c1f99545025ad354857fef849a9f7ace) )
	ROM_LOAD16_BYTE( "9539.bin", 0x000001, 0x020000, CRC(98335286) SHA1(57792f661e82cbb7afe6e1723a419219c7b6e7b7) )
	j6bigwhl_sound
ROM_END

ROM_START( j6bigwhld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bwheelp1", 0x000000, 0x020000, CRC(fc8a8aa3) SHA1(13949e37a89eab9c906d91ca93398b778839011c) )
	ROM_LOAD16_BYTE( "bwheelp2", 0x000001, 0x020000, CRC(67ee23ae) SHA1(0059c40e7379958a71e0d5ba3e17622a879b59ba) )
	j6bigwhl_sound
ROM_END

ROM_START( j6bigwhle )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bigwheel8cash-p1.bin", 0x0000, 0x020000, CRC(9fa585a9) SHA1(10c6c42772bec0e974c86a96029e8cf42c14c983) )
	ROM_LOAD16_BYTE( "bigwheel8cash-p2.bin", 0x0001, 0x020000, CRC(3375b0e0) SHA1(f1d85364ec2dee48ddf1891c96d4059e38e8902e) )
	j6bigwhl_sound
ROM_END

ROM_START( j6monobn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7935.bin", 0x000000, 0x020000, CRC(afdb6320) SHA1(33f49e796f2ee08ebd604caf140f07febdedc0d0) )
	ROM_LOAD16_BYTE( "7936.bin", 0x000001, 0x020000, CRC(782cabbb) SHA1(9103126580923427741a6bb8cea75cf4b7fe78dd) )
	j6bigwhl_sound
ROM_END

ROM_START( j6monobna )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7937.bin", 0x000000, 0x020000, CRC(44453718) SHA1(35dc2238155d9415fe1d4f518bc02368ca27a1a7) )
	ROM_LOAD16_BYTE( "7936.bin", 0x000001, 0x020000, CRC(782cabbb) SHA1(9103126580923427741a6bb8cea75cf4b7fe78dd) )
	j6bigwhl_sound
ROM_END

ROM_START( j6monobnb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7938.bin", 0x000000, 0x020000, CRC(cdb612a8) SHA1(346eecf0c301f9028691055cf83e939fc5ae303f) )
	ROM_LOAD16_BYTE( "7936.bin", 0x000001, 0x020000, CRC(782cabbb) SHA1(9103126580923427741a6bb8cea75cf4b7fe78dd) )
	j6bigwhl_sound
ROM_END



#define j6bnza_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1064.bin", 0x000000, 0x080000, CRC(266edecb) SHA1(c985081bd2a4500889aae0dc7ecd7d8e4cbd1591) )

ROM_START( j6bnza )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6855.bin", 0x000000, 0x020000, CRC(0dc81f2b) SHA1(6bf9e75cf5daa1a16423854bc89e041755dece8e) )
	ROM_LOAD16_BYTE( "6856.bin", 0x000001, 0x020000, CRC(985853b4) SHA1(3492b8ee3de5c90ca110c3d07d28e5efe5bee15b) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzaa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6857.bin", 0x000000, 0x020000, CRC(e6564b13) SHA1(b822a057c342a5995759be3e44e4b476a67457e5) )
	ROM_LOAD16_BYTE( "6856.bin", 0x000001, 0x020000, CRC(985853b4) SHA1(3492b8ee3de5c90ca110c3d07d28e5efe5bee15b) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzab )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6858.bin", 0x000000, 0x020000, CRC(5f0cdf4a) SHA1(72826b95b7a1288e172053f0ad94ca9f24cb556d) )
	ROM_LOAD16_BYTE( "6856.bin", 0x000001, 0x020000, CRC(985853b4) SHA1(3492b8ee3de5c90ca110c3d07d28e5efe5bee15b) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzac )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6859.bin", 0x000000, 0x020000, CRC(0185b11a) SHA1(e6ebab44496ac47cd89c766b3842364b811b4bed) )
	ROM_LOAD16_BYTE( "6856.bin", 0x000001, 0x020000, CRC(985853b4) SHA1(3492b8ee3de5c90ca110c3d07d28e5efe5bee15b) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzad )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8570.bin", 0x000000, 0x020000, CRC(5695eabe) SHA1(ed34a3996fdfcf587ce4c87e6bcd365c89ac922b) )
	ROM_LOAD16_BYTE( "8571.bin", 0x000001, 0x020000, CRC(9358e394) SHA1(37ea22c74bc659305c0212211ba3b79fdb6754d1) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzae )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8572.bin", 0x000000, 0x020000, CRC(bd0bbe86) SHA1(d709f84ea42b77e3f2b51987f1ba4c2d48f3f2ba) )
	ROM_LOAD16_BYTE( "8571.bin", 0x000001, 0x020000, CRC(9358e394) SHA1(37ea22c74bc659305c0212211ba3b79fdb6754d1) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzaf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8573.bin", 0x000000, 0x020000, CRC(04512adf) SHA1(39dbb25c80db04a25c6966a0401daa196d165b8f) )
	ROM_LOAD16_BYTE( "8571.bin", 0x000001, 0x020000, CRC(9358e394) SHA1(37ea22c74bc659305c0212211ba3b79fdb6754d1) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzag )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8616.bin", 0x000000, 0x020000, CRC(5ad8448f) SHA1(32c74f9316b8f79b4e303de64fa29d64ee9a1f54) )
	ROM_LOAD16_BYTE( "8571.bin", 0x000001, 0x020000, CRC(9358e394) SHA1(37ea22c74bc659305c0212211ba3b79fdb6754d1) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzah )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bonv5p1", 0x000000, 0x020000, CRC(cc1363b0) SHA1(b4e5cf134cd25ad99e487209bed2a078fbd644ee) )
	ROM_LOAD16_BYTE( "bonv5p2", 0x000001, 0x020000, CRC(c728b655) SHA1(5a50e5204e64b550fb0ecd6caf4f43cc3d936f1d) )
	j6bnza_sound
ROM_END

ROM_START( j6bnzai )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bon5p18c", 0x000000, 0x020000, CRC(c05ecd81) SHA1(f16f9e6bb44eb0df0c9c7467d8624f1f60884bf4) )
	ROM_LOAD16_BYTE( "bonv5p2", 0x000001, 0x020000, CRC(c728b655) SHA1(5a50e5204e64b550fb0ecd6caf4f43cc3d936f1d) ) // aka bonv5p2
	j6bnza_sound
ROM_END

// there was another ''v8' set but it was actually the same as j6bnzah which is clearly wrong, meaning this rom pairs with nothing..
ROM_START( j6bnzaj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bona8.1i", 0x000000, 0x020000, CRC(e156ba97) SHA1(a5f489313bcc65bb7e49dcabc4ea985a49afcb01) ) // doesn't pair with anything we have..
	ROM_LOAD16_BYTE( "bona8.2",  0x000001, 0x020000, NO_DUMP )
	j6bnza_sound
ROM_END

ROM_START( j6bnzak )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bonz10p1", 0x000000, 0x020000, CRC(96de5d60) SHA1(9a41b514aebf68581d82459040f62f97b10ef02f) )
	ROM_LOAD16_BYTE( "bonz10p2", 0x000001, 0x020000, CRC(475a8098) SHA1(08a6573fe6c9df790b54dc036884b8bdc5e52612) )
	j6bnza_sound
ROM_END

#define j6brkout_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "bksnd.bin", 0x000000, 0x080000, CRC(f72bd4f4) SHA1(ef8651fe7bb5f5340e41d35ca0669cba7d9c1372) )

ROM_START( j6brkout )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bk30cz_04_1.b8", 0x000000, 0x020000, CRC(b6e8611d) SHA1(c2cf0e73d4a3fb94320abe48a76e9fff692b4a9c) )
	ROM_LOAD16_BYTE( "bk30cz_04_2.b8", 0x000001, 0x020000, CRC(b243e4eb) SHA1(093fec6e46c1c26e73011fca1ff8b7f847d27d96) )
	j6brkout_sound
ROM_END

ROM_START( j6brkouta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bk75cz_04_1.b8", 0x000000, 0x020000, CRC(baa5cf2c) SHA1(d24262431b3f15adf784006fd422efb3c79e1197) )
	ROM_LOAD16_BYTE( "bk30cz_04_2.b8", 0x000001, 0x020000, CRC(b243e4eb) SHA1(093fec6e46c1c26e73011fca1ff8b7f847d27d96) )
	j6brkout_sound
ROM_END


ROM_START( j6bucks )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bucksfizz.p1", 0x000000, 0x020000, CRC(6ebfa6e5) SHA1(16a7a0f8e4271edaf95f898bbf4c4f5cb8936e41) )
	ROM_LOAD16_BYTE( "bucksfizz.p2", 0x000001, 0x020000, CRC(2986a6b8) SHA1(4bfdc3828fa5723491a8e8dcb8e48dfea1e897fe) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "bucksfizzsnd.bin", 0x000000, 0x080000, CRC(5f68aabc) SHA1(e8ce0cf8b43337e0bd9e098acadd19480c98c3bf) )
ROM_END






#define j6cpclb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cpalaceclubsnd", 0x000000, 0x080000, CRC(ef433c44) SHA1(049bdbbe8d88fb77dbfc9c2690e62982e7fe20ea) )

ROM_START( j6cpclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cpalaceclubr1", 0x000000, 0x020000, CRC(e556eb51) SHA1(0d63e35b4cf7cbd0f56e2ee9b250d1499cec6614) )
	ROM_LOAD16_BYTE( "cpalaceclubr2", 0x000001, 0x020000, CRC(0d0ca65f) SHA1(b24f4d3127c610191f823899538a89110de471ec) )
	j6cpclb_sound
ROM_END

ROM_START( j6cpclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cpc.1.b8", 0x000000, 0x020000, CRC(42bf4422) SHA1(9f78a61d3cb929c5b8c4a56f34c87a7117e48b96) )
	ROM_LOAD16_BYTE( "cpc2.b8", 0x000001, 0x020000, CRC(7212cd14) SHA1(074830d0bd7519adcdd8906e480c308f38025f7b) )
	j6cpclb_sound
ROM_END

ROM_START( j6cpclbb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cpcp1.b8", 0x000000, 0x020000, CRC(a921101a) SHA1(9a24f8e7ec34225074402a8eeb8ff7d96ee96532) )
	ROM_LOAD16_BYTE( "cpc2.b8", 0x000001, 0x020000, CRC(7212cd14) SHA1(074830d0bd7519adcdd8906e480c308f38025f7b) )
	j6cpclb_sound
ROM_END




#define j6camelt_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "camsound.bin", 0x000000, 0x080000, CRC(70d5a16f) SHA1(0e8ec67387274298637598bf1ab8c479aa108c54) )

ROM_START( j6camelt )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20298.bin", 0x000000, 0x020000, CRC(9d773b08) SHA1(98ce12b4a7983c8b2765358fb3b070249ca25a4a) )
	ROM_LOAD16_BYTE( "20299.bin", 0x000001, 0x020000, CRC(740b93b2) SHA1(cd4ffab8d5c229f236c4771f3d1ff6d7ea94074d) )
	j6camelt_sound
ROM_END

ROM_START( j6camelta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20300.bin", 0x000000, 0x020000, CRC(76e96f30) SHA1(28e65cdc38a93652f4650b48e1c3ff689b13c8bd) )
	ROM_LOAD16_BYTE( "20299.bin", 0x000001, 0x020000, CRC(740b93b2) SHA1(cd4ffab8d5c229f236c4771f3d1ff6d7ea94074d) )
	j6camelt_sound
ROM_END

ROM_START( j6cameltb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20301.bin", 0x000000, 0x020000, CRC(cfb3fb69) SHA1(0ce50a66ff856bcbcb81beb78be14fb3d0ba7b2b) )
	ROM_LOAD16_BYTE( "20299.bin", 0x000001, 0x020000, CRC(740b93b2) SHA1(cd4ffab8d5c229f236c4771f3d1ff6d7ea94074d) )
	j6camelt_sound
ROM_END

ROM_START( j6cameltc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20302.bin", 0x000000, 0x020000, CRC(ff1a4a80) SHA1(82f9856264831cee7b5ee99617767625ec9504be) )
	ROM_LOAD16_BYTE( "20299.bin", 0x000001, 0x020000, CRC(740b93b2) SHA1(cd4ffab8d5c229f236c4771f3d1ff6d7ea94074d) )
	j6camelt_sound
ROM_END

ROM_START( j6cameltd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cam15p1", 0x000000, 0x020000, CRC(88537c3d) SHA1(a0e26a1114fae1e495ae634bf90142ab5953745e) ) // aka camv8p1
	ROM_LOAD16_BYTE( "cam15p2", 0x000001, 0x020000, CRC(048c536d) SHA1(0053e13c86c11ae37cfb1b802b4aeb2b6a34deb8) ) // aka camv8p2
	j6camelt_sound
ROM_END


ROM_START( j6scarlt )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "captscar.p1", 0x000000, 0x020000, CRC(ce9bc3f8) SHA1(ac303b33df4a2b022ebdff2f64ef181c5d59e968) )
	ROM_LOAD16_BYTE( "captscar.p2", 0x000001, 0x020000, CRC(38ff9c39) SHA1(4ec58fe670e7b64352181773c40e42618a96ac51) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "captscarsnd.bin", 0x000000, 0x080000, CRC(89768137) SHA1(1f93afaae31f421d07d840b44e25578a90868910) )
ROM_END

#define j6cshbox_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cbcsnd.bin", 0x000000, 0x080000, CRC(bf209b9b) SHA1(d2c079b05baeae80ed772509c3d9640e682addcd) )

ROM_START( j6cshbox )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cbc2_0.p1", 0x000000, 0x020000, CRC(e2b9b11f) SHA1(cf5cffa6300f569f2e4cca4b3706c9eeeecc7949) )
	ROM_LOAD16_BYTE( "cbc2_0.p2", 0x000001, 0x020000, CRC(8b93ee17) SHA1(515491a68651e57bfd77a4470f100e50b3287c2b) )
	j6cshbox_sound
ROM_END

ROM_START( j6cshboxa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cbc3_6.p1", 0x000000, 0x020000, CRC(25051c70) SHA1(5f60bb99fc1b30f6ef6496a440570f2f09e042a0) )
	ROM_LOAD16_BYTE( "cbc3_6.p2", 0x000001, 0x020000, CRC(78adbff4) SHA1(a4e2817b8df2f56bdb128f7b8987f3cb5592b6e2) )
	j6cshbox_sound
ROM_END

ROM_START( j6cshboxb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cbc4_0.p1", 0x000000, 0x020000, CRC(a0b52d70) SHA1(d23ef23a86b6e3d4dc58c6ba024bde5646749f39) )
	ROM_LOAD16_BYTE( "cbc4_0.p2", 0x000001, 0x020000, CRC(2f2864d6) SHA1(6d50cc57c762bd3ded60356318e70ab5d24cd0b6) )
	j6cshbox_sound
ROM_END



ROM_START( j6cshbeu )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cbe0_3.p1", 0x000000, 0x020000, CRC(184e945c) SHA1(a0659d1ddc9c1a9c973687769103b07b445b85c4) )
	ROM_LOAD16_BYTE( "cbe0_3.p2", 0x000001, 0x020000, CRC(e1f7c860) SHA1(95e0d8b802b1e171b092d509affcd9cfbb10eb80) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "cbcsnd.bin", 0x000000, 0x080000, CRC(bf209b9b) SHA1(d2c079b05baeae80ed772509c3d9640e682addcd) )
ROM_END

#define j6cshbst_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "as1.bin", 0x000000, 0x080000, CRC(96127e49) SHA1(58bec4a024eb557995c67ac81880ad3a9de84ac0) )

ROM_START( j6cshbst )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20341.bin", 0x000000, 0x020000, CRC(b1935c10) SHA1(6b3555d66205cd7f9954bfbab16ac80d70781e3d) )
	ROM_LOAD16_BYTE( "20342.bin", 0x000001, 0x020000, CRC(236e20e4) SHA1(12d1e726ed4ab41cff4ff1fa94203ee684c3f763) )
	j6cshbst_sound
ROM_END

ROM_START( j6cshbsta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20343.bin", 0x000000, 0x020000, CRC(5a0d0828) SHA1(b39460e9d83d240eaec830222a3229d23db0e8d4) )
	ROM_LOAD16_BYTE( "20342.bin", 0x000001, 0x020000, CRC(236e20e4) SHA1(12d1e726ed4ab41cff4ff1fa94203ee684c3f763) )
	j6cshbst_sound
ROM_END

ROM_START( j6cshbstb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20344.bin", 0x000000, 0x020000, CRC(e3579c71) SHA1(3df10417be704d8487d75de42fda6656f3eb2705) )
	ROM_LOAD16_BYTE( "20342.bin", 0x000001, 0x020000, CRC(236e20e4) SHA1(12d1e726ed4ab41cff4ff1fa94203ee684c3f763) )
	j6cshbst_sound
ROM_END

ROM_START( j6cshbstc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20345.bin", 0x000000, 0x020000, CRC(d3fe2d98) SHA1(97d635372bb30f90334599155b0648a8c7c40768) )
	ROM_LOAD16_BYTE( "20342.bin", 0x000001, 0x020000, CRC(236e20e4) SHA1(12d1e726ed4ab41cff4ff1fa94203ee684c3f763) )
	j6cshbst_sound
ROM_END

ROM_START( j6cshbstd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cbus15p1", 0x000000, 0x020000, CRC(152b259e) SHA1(3fa8c6465c7fc06f6de390386dccc34fc2545d2e) )
	ROM_LOAD16_BYTE( "cbus15p2", 0x000001, 0x020000, CRC(20feeb0f) SHA1(29a73ac17f24cba17d1efe6d354b9d775fc5e244) )
	j6cshbst_sound
ROM_END


#define j6cshcnt_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cd75snd.bin", 0x000000, 0x080000, CRC(352e28cd) SHA1(c98307f5eaf511c9d281151d1c07ffd83f24244c) )

ROM_START( j6cshcnt )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cd30cz07_1.b8", 0x000000, 0x020000, CRC(62a81565) SHA1(302f8887e4453b88b623a100dbaecca11a261eae) )
	ROM_LOAD16_BYTE( "cd30cz07_2.b8", 0x000001, 0x020000, CRC(63d0ca74) SHA1(a5a146c3463f555e77efbbf7eea65c6edc7ce37b) )
	j6cshcnt_sound
ROM_END

ROM_START( j6cshcnta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cd751.bin", 0x000000, 0x020000, CRC(6ee5bb54) SHA1(9f9d693fb06ae06512b568656855b204b242a4ec) ) // aka cd75cz07_1.b8
	ROM_LOAD16_BYTE( "cd30cz07_2.b8", 0x000001, 0x020000, CRC(63d0ca74) SHA1(a5a146c3463f555e77efbbf7eea65c6edc7ce37b) ) // aka cd75cz07_2.b8, cd752.bin
	j6cshcnt_sound
ROM_END

#define j6cshrd_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cashraider8b11as1 24-9-98.bin", 0x000000, 0x080000, CRC(8d28ba3c) SHA1(5d403bdc4cfd6a3e14c1e9458dcda112ed1770c5) ) /* aka carasnd.bin */

ROM_START( j6cshrd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cara-4a1.bin", 0x000000, 0x020000, CRC(6deb5704) SHA1(518ccc9f5c88e993ac430db1f3c200669da6fbd4) )
	ROM_LOAD16_BYTE( "cara-4a2.bin", 0x000001, 0x020000, CRC(b2d2acde) SHA1(b4e5ad405d9ab85901122fe3d10f29cd5b14ab89) ) // aka cara-4s2.bin
	j6cshrd_sound
ROM_END

ROM_START( j6cshrda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cara-4p1.bin", 0x000000, 0x020000, CRC(d4b1c35d) SHA1(ed3e99619731d4920c800c34e07f9a7d65d75605) )
	ROM_LOAD16_BYTE( "cara-4a2.bin", 0x000001, 0x020000, CRC(b2d2acde) SHA1(b4e5ad405d9ab85901122fe3d10f29cd5b14ab89) )
	j6cshrd_sound
ROM_END

ROM_START( j6cshrdb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cara-4s1.bin", 0x000000, 0x020000, CRC(3f2f9765) SHA1(966eeee083901151f17ce03e12acc76ad5804591) )
	ROM_LOAD16_BYTE( "cara-4a2.bin", 0x000001, 0x020000, CRC(b2d2acde) SHA1(b4e5ad405d9ab85901122fe3d10f29cd5b14ab89) )
	j6cshrd_sound
ROM_END

ROM_START( j6cshrdc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cara-4wp.bin", 0x000000, 0x020000, CRC(b6dcb2d5) SHA1(dc542e3838e08b40f9e3dac2d7df9dfc841a519b) )
	ROM_LOAD16_BYTE( "cara-4a2.bin", 0x000001, 0x020000, CRC(b2d2acde) SHA1(b4e5ad405d9ab85901122fe3d10f29cd5b14ab89) )
	j6cshrd_sound
ROM_END

ROM_START( j6cshrdd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cash raiders std 9661.bin", 0x000000, 0x020000, CRC(b97d1093) SHA1(e36d2c924e66f5868026bc0f1e31df955a49f874) )
	ROM_LOAD16_BYTE( "cash raiders p2 23c4.bin", 0x000001, 0x020000, CRC(d61e26bf) SHA1(282d9455a8ab401a0eb04703b5e1b8f9ab37bfea) )
	j6cshrd_sound
ROM_END




ROM_START( j6cshtwr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cashtowers.p1", 0x000000, 0x020000, CRC(43a3d2bc) SHA1(5f96fef540075d3ded635975e4638dd4ab41cedf) )
	ROM_LOAD16_BYTE( "cashtowers.p2", 0x000001, 0x020000, CRC(fe4cd1d6) SHA1(4b45f9c84b7c23fd2176bd5a0bb7b38c0f08db7a) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "cashtowerssnd.bin", 0x000000, 0x080000, CRC(c51444db) SHA1(8c5b726cab00f73f9efb074ca060c29c80c4348b) )
ROM_END


#define j6cshvgs_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cvssnd.bin", 0x000000, 0x080000, CRC(f2f828f5) SHA1(3d141884ea68d3e440ac43eaec3e8133fa8ae776) )

ROM_START( j6cshvgs )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cast-31.bin", 0x000000, 0x020000, CRC(5b887503) SHA1(a1484bf3d8bdc354be78b5bdd83bb0fc5933d55a) )
	ROM_LOAD16_BYTE( "cast-32.bin", 0x000001, 0x020000, CRC(ddfe1279) SHA1(9131786a3861185f71bfb00be0ae98a907315c9a) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cast-3a1.bin", 0x000000, 0x020000, CRC(094cb562) SHA1(48d6a068ceb09580f0d6f4ccc71782ea18074d09) )
	ROM_LOAD16_BYTE( "cast-32.bin", 0x000001, 0x020000, CRC(ddfe1279) SHA1(9131786a3861185f71bfb00be0ae98a907315c9a) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cast-3n1.bin", 0x000000, 0x020000, CRC(6abecdc7) SHA1(ec227d63c0b309564c48412566a6f1d38282a8b8) )
	ROM_LOAD16_BYTE( "cast-32.bin", 0x000001, 0x020000, CRC(ddfe1279) SHA1(9131786a3861185f71bfb00be0ae98a907315c9a) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cast-3p1.bin", 0x000000, 0x020000, CRC(b016213b) SHA1(ca90373912f1b35d23ef9e8441348907529a074d) )
	ROM_LOAD16_BYTE( "cast-32.bin", 0x000001, 0x020000, CRC(ddfe1279) SHA1(9131786a3861185f71bfb00be0ae98a907315c9a) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cast3np1.bin", 0x000000, 0x020000, CRC(812099ff) SHA1(d63d87578866140c7a7f09c0d52ebc04cd42f087) )
	ROM_LOAD16_BYTE( "cast-32.bin", 0x000001, 0x020000, CRC(ddfe1279) SHA1(9131786a3861185f71bfb00be0ae98a907315c9a) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgse )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3749", 0x000000, 0x020000, CRC(6ad9defb) SHA1(098cb32aea036321f9fe30067376856072e31d00) )
	ROM_LOAD16_BYTE( "cc85", 0x000001, 0x020000, CRC(a7343977) SHA1(1de3372e1f9af30968c43d2c8dbacc40a115da71) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3781", 0x000000, 0x020000, CRC(092ba65e) SHA1(6376ab330465f0690fcd80dae3f9cc848fc47fba) )
	ROM_LOAD16_BYTE( "cc85", 0x000001, 0x020000, CRC(a7343977) SHA1(1de3372e1f9af30968c43d2c8dbacc40a115da71) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3788", 0x000000, 0x020000, CRC(d3834aa2) SHA1(4d89f0c020be8731fa991c97d9a81657835187f7) )
	ROM_LOAD16_BYTE( "cc85", 0x000001, 0x020000, CRC(a7343977) SHA1(1de3372e1f9af30968c43d2c8dbacc40a115da71) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3789", 0x000000, 0x020000, CRC(381d1e9a) SHA1(7ee4e4f5aa01267e89af626dba819302933de0fc) )
	ROM_LOAD16_BYTE( "cc85", 0x000001, 0x020000, CRC(a7343977) SHA1(1de3372e1f9af30968c43d2c8dbacc40a115da71) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-11.bin", 0x000000, 0x020000, CRC(895e45db) SHA1(15e2c51a08827322ed34220178476d87cce0b615) )
	ROM_LOAD16_BYTE( "cvs-12.bin", 0x000001, 0x020000, CRC(97aa7764) SHA1(d9a9db1112ccd2721a52ab45de9c0dc85ba027a1) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-1a1.bin", 0x000000, 0x020000, CRC(db9a85ba) SHA1(9f459629aebcb4842e726c6c55427cd253e5eca8) )
	ROM_LOAD16_BYTE( "cvs-12.bin", 0x000001, 0x020000, CRC(97aa7764) SHA1(d9a9db1112ccd2721a52ab45de9c0dc85ba027a1) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-1n1.bin", 0x000000, 0x020000, CRC(b868fd1f) SHA1(3aefc4f96ea4dc386f98dd2fd70626c1b7e49c54) )
	ROM_LOAD16_BYTE( "cvs-12.bin", 0x000001, 0x020000, CRC(97aa7764) SHA1(d9a9db1112ccd2721a52ab45de9c0dc85ba027a1) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-1np1.bin", 0x000000, 0x020000, CRC(53f6a927) SHA1(6ba558699a3e64c7f785d963174e114dc208c253) )
	ROM_LOAD16_BYTE( "cvs-12.bin", 0x000001, 0x020000, CRC(97aa7764) SHA1(d9a9db1112ccd2721a52ab45de9c0dc85ba027a1) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-1p1.bin", 0x000000, 0x020000, CRC(62c011e3) SHA1(6d075f07583588f57ba9b21404e4dc292ebac1db) )
	ROM_LOAD16_BYTE( "cvs-12.bin", 0x000001, 0x020000, CRC(97aa7764) SHA1(d9a9db1112ccd2721a52ab45de9c0dc85ba027a1) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-41.bin", 0x000000, 0x020000, CRC(48c01ca7) SHA1(17d961b43cf7dc65fc67f3991ecd8839314885bc) )
	ROM_LOAD16_BYTE( "cvs-42.bin", 0x000001, 0x020000, CRC(d30d313c) SHA1(f3b3195e376702d9239d520e1bfe5733c91f9164) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgso )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-4a1.bin", 0x000000, 0x020000, CRC(1a04dcc6) SHA1(6a1134aaa86ef37b4a092b8e6893f34d88a313a7) )
	ROM_LOAD16_BYTE( "cvs-42.bin", 0x000001, 0x020000, CRC(d30d313c) SHA1(f3b3195e376702d9239d520e1bfe5733c91f9164) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsp )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-4l1.bin", 0x000000, 0x020000, CRC(79f6a463) SHA1(5d48edc10dba3e4db3dad260bdc038aeeeababeb) )
	ROM_LOAD16_BYTE( "cvs-42.bin", 0x000001, 0x020000, CRC(d30d313c) SHA1(f3b3195e376702d9239d520e1bfe5733c91f9164) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsq )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-4lp1.bin", 0x000000, 0x020000, CRC(9268f05b) SHA1(d2fb3ce71943fd07f16887c414e3313b3fe2094e) )
	ROM_LOAD16_BYTE( "cvs-42.bin", 0x000001, 0x020000, CRC(d30d313c) SHA1(f3b3195e376702d9239d520e1bfe5733c91f9164) )
	j6cshvgs_sound
ROM_END

ROM_START( j6cshvgsr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cvs-4p1.bin", 0x000000, 0x020000, CRC(a35e489f) SHA1(f652e1380a5b8a5c7953d88d1bcc51e94bb05f98) )
	ROM_LOAD16_BYTE( "cvs-42.bin", 0x000001, 0x020000, CRC(d30d313c) SHA1(f3b3195e376702d9239d520e1bfe5733c91f9164) )
	j6cshvgs_sound
ROM_END

#define j6cas5_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cfl-snd.bin", 0x000000, 0x080000, CRC(0016ab04) SHA1(82d133f485b325b29db901f6254c80ca959abd3e) )

ROM_START( j6cas5 )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-2_1.bin", 0x000000, 0x020000, CRC(91cd1258) SHA1(a6fc4be9754b906eef25995207eb62f60988e0dc) )
	ROM_LOAD16_BYTE( "ca5l-2_2.bin", 0x000001, 0x020000, CRC(1e69935e) SHA1(6322dbb7a9ed31a225355dc1b4fa951f2ee863bc) )
	j6cas5_sound
ROM_END


ROM_START( j6cas5a )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-2p1.bin", 0x000000, 0x020000, CRC(7a534660) SHA1(25be6e8d31af60c87227c536236157ff4154ea2e) )
	ROM_LOAD16_BYTE( "ca5l-2_2.bin", 0x000001, 0x020000, CRC(1e69935e) SHA1(6322dbb7a9ed31a225355dc1b4fa951f2ee863bc) )
	j6cas5_sound
ROM_END


ROM_START( j6cas5b )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-3_1.bin", 0x000000, 0x020000, CRC(1a15f55c) SHA1(9fd2198a644a843a64e9ee22825f23b4cfbcc11e) )
	ROM_LOAD16_BYTE( "ca5l-3_2.bin", 0x000001, 0x020000, CRC(ae6403f9) SHA1(56b85018f9548f7f3e64112fde7d5cb7d619ef72) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5c )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-3p1.bin", 0x000000, 0x020000, CRC(f18ba164) SHA1(419efdd6a4fde98d46d0ed67cd1373d4c8b97702) )
	ROM_LOAD16_BYTE( "ca5l-3_2.bin", 0x000001, 0x020000, CRC(ae6403f9) SHA1(56b85018f9548f7f3e64112fde7d5cb7d619ef72) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5d )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-4_1.bin", 0x000000, 0x020000, CRC(cff457d4) SHA1(c57cfadd07ae93deb435e19834ccf116d8f235b5) )
	ROM_LOAD16_BYTE( "ca5l-4_2.bin", 0x000001, 0x020000, CRC(d0db6d9c) SHA1(2c225a099fdfc45bc0c04a1a698259cb25db8db3) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5e )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-4a1.bin", 0x000000, 0x020000, CRC(9d3097b5) SHA1(c0b538bb17d14af08e7f68b4d9c7cc1bdfb1d53f) )
	ROM_LOAD16_BYTE( "ca5l-4_2.bin", 0x000001, 0x020000, CRC(d0db6d9c) SHA1(2c225a099fdfc45bc0c04a1a698259cb25db8db3) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5f )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-4p1.bin", 0x000000, 0x020000, CRC(246a03ec) SHA1(ffe8723390638982bcc7d5309c31f6049cf3f8cf) )
	ROM_LOAD16_BYTE( "ca5l-4_2.bin", 0x000001, 0x020000, CRC(d0db6d9c) SHA1(2c225a099fdfc45bc0c04a1a698259cb25db8db3) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5g )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ca5l-4w1.bin", 0x000000, 0x020000, CRC(ad99265c) SHA1(dec9e882045933b7d52c376340a0ca7fd9bb477c) )
	ROM_LOAD16_BYTE( "ca5l-4_2.bin", 0x000001, 0x020000, CRC(d0db6d9c) SHA1(2c225a099fdfc45bc0c04a1a698259cb25db8db3) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5h )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cfli-51.bin", 0x000000, 0x020000, CRC(b9f1b8dc) SHA1(ae37f689884c1163da5ce76dbafca48c78dc735f) )
	ROM_LOAD16_BYTE( "cfli-52.bin", 0x000001, 0x020000, CRC(c34ab739) SHA1(0289061bfaf96445c35ee11a6d494c23e0d7efc1) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5i )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cfli-5a1.bin", 0x000000, 0x020000, CRC(eb3578bd) SHA1(83a65664585eced36ab24c6853f6f97d11dd2beb) )
	ROM_LOAD16_BYTE( "cfli-52.bin", 0x000001, 0x020000, CRC(c34ab739) SHA1(0289061bfaf96445c35ee11a6d494c23e0d7efc1) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5j )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cfli-5p1.bin", 0x000000, 0x020000, CRC(526fece4) SHA1(cb407bb9de53ac0b967e5568db1dd20672be37c1) )
	ROM_LOAD16_BYTE( "cfli-52.bin", 0x000001, 0x020000, CRC(c34ab739) SHA1(0289061bfaf96445c35ee11a6d494c23e0d7efc1) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5k )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cfli-5w1.bin", 0x000000, 0x020000, CRC(db9cc954) SHA1(da44fb85c15c970c0e0fb75ed0cd0f652d1b8d82) )
	ROM_LOAD16_BYTE( "cfli-52.bin", 0x000001, 0x020000, CRC(c34ab739) SHA1(0289061bfaf96445c35ee11a6d494c23e0d7efc1) )
	j6cas5_sound
ROM_END

ROM_START( j6cas5l )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cas5line.p1", 0x00000, 0x020000, CRC(def7635e) SHA1(d47fb7d3d1212892fb4ca7c6deb508ba79b2d665) )
	ROM_LOAD16_BYTE( "cas5line.p2", 0x00001, 0x020000, CRC(7004a4d1) SHA1(003809bdaad1a6c9b72d2c16a2203bc62bc065a5) )
	j6cas5_sound
ROM_END


#define j6cascz_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1274.bin", 0x000000, 0x080000, CRC(90a19aaa) SHA1(7b17e9fda01d4fb163e09107759a6bf473fc6dc0) ) \
	ROM_LOAD( "crazycasinosnd.bin", 0x000000, 0x080000, CRC(d10b8005) SHA1(e499e4e119956c7831dcec8dc8a6e338423afafb) )
// check hw for j6cascz sets


ROM_START( j6cascz )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7648.bin", 0x000000, 0x020000, CRC(ac772dd4) SHA1(c288cdcf678edccdca7c24d62bb0d5b1bdddeae2) )
	ROM_LOAD16_BYTE( "7649.bin", 0x000001, 0x020000, CRC(9184a594) SHA1(a7db14191b04e7e9fba1f167157af0f187f2d591) )
	j6cascz_sound
ROM_END

ROM_START( j6cascza )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7650.bin", 0x000000, 0x020000, CRC(47e979ec) SHA1(9a0e9e0e43c720d6095e75dd735d98210e3fbc00) )
	ROM_LOAD16_BYTE( "7649.bin", 0x000001, 0x020000, CRC(9184a594) SHA1(a7db14191b04e7e9fba1f167157af0f187f2d591) )
	j6cascz_sound
ROM_END

ROM_START( j6casczb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7651.bin", 0x000000, 0x020000, CRC(feb3edb5) SHA1(9c6a33ce799a4baebc8f0aae8af6f7fba4697679) )
	ROM_LOAD16_BYTE( "7649.bin", 0x000001, 0x020000, CRC(9184a594) SHA1(a7db14191b04e7e9fba1f167157af0f187f2d591) )
	j6cascz_sound
ROM_END

ROM_START( j6casczc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8159.bin", 0x000000, 0x020000, CRC(36904fce) SHA1(b949fdba64d1642495ef3542f65f3b21765b6bc1) )
	ROM_LOAD16_BYTE( "8160.bin", 0x000001, 0x020000, CRC(165670b8) SHA1(ad78d8fb8a3c8b4a6cf5d62a0aae8cb35f3ebb8b) )
	j6cascz_sound
ROM_END

ROM_START( j6casczd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8162.bin", 0x000000, 0x020000, CRC(64548faf) SHA1(b818d69cd172492137335e32fe892359f584b94d) )
	ROM_LOAD16_BYTE( "8160.bin", 0x000001, 0x020000, CRC(165670b8) SHA1(ad78d8fb8a3c8b4a6cf5d62a0aae8cb35f3ebb8b) )
	j6cascz_sound
ROM_END

ROM_START( j6cascze )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9504.bin", 0x000000, 0x020000, CRC(b49c7755) SHA1(a29c87c9b233041bc22b7a4124750947c5c95bba) )
	ROM_LOAD16_BYTE( "9505.bin", 0x000001, 0x020000, CRC(ab03c1ee) SHA1(2ad919fddf3917b0c112be1b0231e22863b33c20) )
	j6cascz_sound
ROM_END

ROM_START( j6casczf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9506.bin", 0x000000, 0x020000, CRC(5f02236d) SHA1(6ded85fa771d4b27d597f7675b535c21dfafd222) )
	ROM_LOAD16_BYTE( "9505.bin", 0x000001, 0x020000, CRC(ab03c1ee) SHA1(2ad919fddf3917b0c112be1b0231e22863b33c20) )
	j6cascz_sound
ROM_END

ROM_START( j6casczg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9507.bin", 0x000000, 0x020000, CRC(e658b734) SHA1(3bc8d74e4ec0eb4757c8f5ea43edd638b66edab8) )
	ROM_LOAD16_BYTE( "9505.bin", 0x000001, 0x020000, CRC(ab03c1ee) SHA1(2ad919fddf3917b0c112be1b0231e22863b33c20) )
	j6cascz_sound
ROM_END

ROM_START( j6casczh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9508.bin", 0x000000, 0x020000, CRC(d6f106dd) SHA1(a60b01a23fe01e7cb1b009149bbcb6f9dd1ecee5) )
	ROM_LOAD16_BYTE( "9505.bin", 0x000001, 0x020000, CRC(ab03c1ee) SHA1(2ad919fddf3917b0c112be1b0231e22863b33c20) )
	j6cascz_sound
ROM_END


ROM_START( j6casczi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cacr15p1", 0x000000, 0x020000, CRC(6e9b193d) SHA1(a129d741974804f34526902430738ff8adc40bea) )
	ROM_LOAD16_BYTE( "cacr15p2", 0x000001, 0x020000, CRC(a6f43b86) SHA1(18ad61127243a6c02f800dfe87942da1b22f1a5c) )
	j6cascz_sound
ROM_END

ROM_START( j6casczj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ccrv12p1", 0x000000, 0x020000, CRC(b37544eb) SHA1(b684406bd95b76b5705e3980e143cbb2d0c35caa) )
	ROM_LOAD16_BYTE( "ccrv12p2", 0x000001, 0x020000, CRC(34c13ba3) SHA1(d20b7565b1183e95b68cd6f300953bec1d0c8086) ) // aka ccv12p2.bin
	j6cascz_sound
ROM_END

ROM_START( j6casczk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ccv12p1.bin", 0x000000, 0x020000, CRC(e1b1848a) SHA1(a5060b695029fab63f9b2ce3588b5d60e2be3682) )
	ROM_LOAD16_BYTE( "ccrv12p2", 0x000001, 0x020000, CRC(34c13ba3) SHA1(d20b7565b1183e95b68cd6f300953bec1d0c8086) ) // aka ccv12p2.bin
	j6cascz_sound
ROM_END

ROM_START( j6casczl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crazycasino.p1", 0x000000, 0x020000, CRC(8306dded) SHA1(c52c1283a83bfd7c25d80173722f33f53c404c52) )
	ROM_LOAD16_BYTE( "crazycasino.p2", 0x000001, 0x020000, CRC(7ef38dcb) SHA1(57b0110876b842065ff88806eec901daf6a06318) )
	j6cascz_sound
ROM_END

ROM_START( j6casczm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ccrzy8p1", 0x000000, 0x020000, CRC(5057f874) SHA1(c609384f5673d7e86d55c9b4e73dc43fba017dd9) )
	ROM_LOAD16_BYTE( "ccrzy8p2", 0x000001, 0x020000, CRC(4d5c4236) SHA1(a69de914a6fc6b73eac29eacfaa105e5dfd3fc87) )
	j6cascz_sound
ROM_END



#define j6cccla_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6cccla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-21.bin", 0x000000, 0x020000, CRC(95be44b1) SHA1(3057eb6b9e844e9ae025d657a4c59443590dfc84) )
	ROM_LOAD16_BYTE( "cccc-22.bin", 0x000001, 0x020000, CRC(6ba426e1) SHA1(0a6610b6f8383e64f342a029136944b385e611c4) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclaa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-2f1.bin", 0x000000, 0x020000, CRC(5164a7a1) SHA1(1701df4590125822a68d0a4d06ec559e78785cd9) )
	ROM_LOAD16_BYTE( "cccc-22.bin", 0x000001, 0x020000, CRC(6ba426e1) SHA1(0a6610b6f8383e64f342a029136944b385e611c4) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclab )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-2n1.bin", 0x000000, 0x020000, CRC(f7d33539) SHA1(9b92ce4993e1d36b9606e42ce4e6c4340182949d) )
	ROM_LOAD16_BYTE( "cccc-22.bin", 0x000001, 0x020000, CRC(6ba426e1) SHA1(0a6610b6f8383e64f342a029136944b385e611c4) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclac )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-2p1.bin", 0x000000, 0x020000, CRC(7e201089) SHA1(146fdde9bd24bf850c5c673fd4a72ae5a561e5cd) )
	ROM_LOAD16_BYTE( "cccc-22.bin", 0x000001, 0x020000, CRC(6ba426e1) SHA1(0a6610b6f8383e64f342a029136944b385e611c4) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclad )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc2fn1.bin", 0x000000, 0x020000, CRC(3309d629) SHA1(8fc3e20e6340ec36166bd12eb48943ed71ddcb88) )
	ROM_LOAD16_BYTE( "cccc-22.bin", 0x000001, 0x020000, CRC(6ba426e1) SHA1(0a6610b6f8383e64f342a029136944b385e611c4) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclae )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-31.bin", 0x000000, 0x020000, CRC(31682e84) SHA1(3980b5d5a865ef9fabd68c7c382e76998d6bcdc3) )
	ROM_LOAD16_BYTE( "cccc-32.bin", 0x000001, 0x020000, CRC(a61a20a0) SHA1(e216cce234d8ceef25078bbbe90a72fca4de8d52) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclaf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-3b1.bin", 0x000000, 0x020000, CRC(3d2580b5) SHA1(51517f69f6cfc26e698a31e40deaeb003f452af8) )
	ROM_LOAD16_BYTE( "cccc-32.bin", 0x000001, 0x020000, CRC(a61a20a0) SHA1(e216cce234d8ceef25078bbbe90a72fca4de8d52) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclag )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-3e1.bin", 0x000000, 0x020000, CRC(29f372e6) SHA1(4400374824d678a12bfa6068d4a7fea3b29e436e) )
	ROM_LOAD16_BYTE( "cccc-32.bin", 0x000001, 0x020000, CRC(a61a20a0) SHA1(e216cce234d8ceef25078bbbe90a72fca4de8d52) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclah )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-3f1.bin", 0x000000, 0x020000, CRC(f5b2cd94) SHA1(ce7dbab1fadddc1c2d6f3b1441339385104769fe) )
	ROM_LOAD16_BYTE( "cccc-32.bin", 0x000001, 0x020000, CRC(a61a20a0) SHA1(e216cce234d8ceef25078bbbe90a72fca4de8d52) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclai )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-3n1.bin", 0x000000, 0x020000, CRC(53055f0c) SHA1(eb2d848d562d5aa4f7557d5e84ec82b6452c7772) )
	ROM_LOAD16_BYTE( "cccc-32.bin", 0x000001, 0x020000, CRC(a61a20a0) SHA1(e216cce234d8ceef25078bbbe90a72fca4de8d52) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclaj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-3p1.bin", 0x000000, 0x020000, CRC(daf67abc) SHA1(c9750156a5ac02894bfd2a9d7412b47cb0e18aae) )
	ROM_LOAD16_BYTE( "cccc-32.bin", 0x000001, 0x020000, CRC(a61a20a0) SHA1(e216cce234d8ceef25078bbbe90a72fca4de8d52) )
	j6cccla_sound
ROM_END

ROM_START( j6ccclak )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc3fb1.bin", 0x000000, 0x020000, CRC(f9ff63a5) SHA1(08795ebb856302b391288512c4c378cc37f62326) )
	ROM_LOAD16_BYTE( "cccc-32.bin", 0x000001, 0x020000, CRC(a61a20a0) SHA1(e216cce234d8ceef25078bbbe90a72fca4de8d52) )
	j6cccla_sound
ROM_END



#define j6cascla_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6cascla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-11.bin", 0x000000, 0x020000, CRC(921262ff) SHA1(54f71f0e66e7d484c178daa4959ba6aff75230ba) )
	ROM_LOAD16_BYTE( "cccc-12.bin", 0x000001, 0x020000, CRC(98605d3e) SHA1(80dcc318ddcb46982da544ec43b8045692b21f70) )
	j6cascla_sound
ROM_END

ROM_START( j6casclaa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-1f1.bin", 0x000000, 0x020000, CRC(56c881ef) SHA1(b7dfb4a808c032f8806cadc82bc44b3c009832f0) )
	ROM_LOAD16_BYTE( "cccc-12.bin", 0x000001, 0x020000, CRC(98605d3e) SHA1(80dcc318ddcb46982da544ec43b8045692b21f70) )
	j6cascla_sound
ROM_END

ROM_START( j6casclab )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-1n1.bin", 0x000000, 0x020000, CRC(f07f1377) SHA1(009f3f5eefd200a86b5720d2ca34cd193d05cc69) )
	ROM_LOAD16_BYTE( "cccc-12.bin", 0x000001, 0x020000, CRC(98605d3e) SHA1(80dcc318ddcb46982da544ec43b8045692b21f70) )
	j6cascla_sound
ROM_END

ROM_START( j6casclac )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-1p1.bin", 0x000000, 0x020000, CRC(798c36c7) SHA1(01653581142ed5dd8cb9fcb69a16baf8aa5b6ca6) )
	ROM_LOAD16_BYTE( "cccc-12.bin", 0x000001, 0x020000, CRC(98605d3e) SHA1(80dcc318ddcb46982da544ec43b8045692b21f70) )
	j6cascla_sound
ROM_END

ROM_START( j6casclad )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ccc1enp1.bin", 0x000000, 0x020000, CRC(037a1b2d) SHA1(8838a56d08b95efb23e3137a22e2eb64fcacf812) )
	ROM_LOAD16_BYTE( "cccc-12.bin", 0x000001, 0x020000, CRC(98605d3e) SHA1(80dcc318ddcb46982da544ec43b8045692b21f70) )
	j6cascla_sound
ROM_END


ROM_START( j6casclae )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc1np1.bin", 0x000000, 0x020000, CRC(1be1474f) SHA1(9a032b28ef7ec70000d9f7e14624cae0b39501b7) )
	ROM_LOAD16_BYTE( "cccc-12.bin", 0x000001, 0x020000, CRC(98605d3e) SHA1(80dcc318ddcb46982da544ec43b8045692b21f70) )
	j6cascla_sound
ROM_END

ROM_START( j6casclaf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-c1.bin", 0x000000, 0x020000, CRC(959e802d) SHA1(73d32ad724da30131d3d7125bdb6b74a49e6e29e) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclag )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-cb1.bin", 0x000000, 0x020000, CRC(8148727e) SHA1(022d44edc60a8df6874f34973734515f5a4b180b) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclah )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-ce1.bin", 0x000000, 0x020000, CRC(8d05dc4f) SHA1(9308f51e64415688a87185b9258d70ae82045f2e) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclai )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-cf1.bin", 0x000000, 0x020000, CRC(5144633d) SHA1(c52c512adddc90821aac85743f12acae34c82320) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclaj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-cn1.bin", 0x000000, 0x020000, CRC(f7f3f1a5) SHA1(ebbff1909e91ce708a97f4ed2b5811c47951d27a) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclak )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccc-cp1.bin", 0x000000, 0x020000, CRC(7e00d415) SHA1(d2f08457cd638479bb7c3c7a2673894b59e727c3) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclal )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccccnp1.bin", 0x000000, 0x020000, CRC(1c6da59d) SHA1(ab77a898e000c2fecf8ef266390f1aea399d763d) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclam )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cccccep1.bin", 0x000000, 0x020000, CRC(669b8877) SHA1(48372a46ff03d361e3f04bd224ce887c84689aaf) )
	ROM_LOAD16_BYTE( "cccc-c2.bin", 0x000001, 0x020000, CRC(c91c14ce) SHA1(a0aea950f45e0e110b5a8ff5e12590dd5822f31b) )
	j6cascla_sound
ROM_END

ROM_START( j6casclan )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "badf", 0x000000, 0x020000, CRC(053fe05a) SHA1(3f2c3531ecfbb5b593709778023668e1bb49663a) )
	ROM_LOAD16_BYTE( "f983", 0x000001, 0x020000, CRC(f58c0577) SHA1(8dcbc071f4f11d03bff8d0e9656e4e1991423c56) )
	j6cascla_sound
ROM_END

ROM_START( j6casclao )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "baee", 0x000000, 0x020000, CRC(481626fa) SHA1(923a8c6da5033716ae999be4680d1db7325f42ff) )
	ROM_LOAD16_BYTE( "f983", 0x000001, 0x020000, CRC(f58c0577) SHA1(8dcbc071f4f11d03bff8d0e9656e4e1991423c56) )
	j6cascla_sound
ROM_END

ROM_START( j6casclap )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "baef", 0x000000, 0x020000, CRC(a38872c2) SHA1(6888e5c5261edda721eed18924dc4f4b1bde8268) )
	ROM_LOAD16_BYTE( "f983", 0x000001, 0x020000, CRC(f58c0577) SHA1(8dcbc071f4f11d03bff8d0e9656e4e1991423c56) )
	j6cascla_sound
ROM_END

ROM_START( j6casclaq )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "baf7", 0x000000, 0x020000, CRC(f0d3bb8e) SHA1(bbe9123fa9b1eb3640f6ee270ae88223178a8278) )
	ROM_LOAD16_BYTE( "f983", 0x000001, 0x020000, CRC(f58c0577) SHA1(8dcbc071f4f11d03bff8d0e9656e4e1991423c56) )
	j6cascla_sound
ROM_END

ROM_START( j6casclar )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bafb", 0x000000, 0x020000, CRC(d97e5f28) SHA1(589845eaef52cfacb8a07c803c431e3a743c70e8) )
	ROM_LOAD16_BYTE( "f983", 0x000001, 0x020000, CRC(f58c0577) SHA1(8dcbc071f4f11d03bff8d0e9656e4e1991423c56) )
	j6cascla_sound
ROM_END

ROM_START( j6casclas )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bafe", 0x000000, 0x020000, CRC(2a7b5772) SHA1(68e4cd32553c70aef8c301ea3b6bf738f59211f3) )
	ROM_LOAD16_BYTE( "f983", 0x000001, 0x020000, CRC(f58c0577) SHA1(8dcbc071f4f11d03bff8d0e9656e4e1991423c56) )
	j6cascla_sound
ROM_END

ROM_START( j6casclat )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "baff", 0x000000, 0x020000, CRC(c1e5034a) SHA1(4716069d318fee3e948ff2e4f1ad743b9477070c) )
	ROM_LOAD16_BYTE( "f983", 0x000001, 0x020000, CRC(f58c0577) SHA1(8dcbc071f4f11d03bff8d0e9656e4e1991423c56) )
	j6cascla_sound
ROM_END

#define j6casclb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1274.bin", 0x000000, 0x080000, CRC(90a19aaa) SHA1(7b17e9fda01d4fb163e09107759a6bf473fc6dc0) ) \
	ROM_LOAD( "cccsnd.bin", 0x000000, 0x080000, CRC(facc0580) SHA1(4f0307a6439e5df97ee4e80f6300e7bf056f3dad) )

ROM_START( j6casclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20059.bin", 0x000000, 0x020000, CRC(523d8116) SHA1(5a2763d7095ab51e3d660f700d53d225594ff6ca) )
	ROM_LOAD16_BYTE( "20060.bin", 0x000001, 0x020000, CRC(7da19bcc) SHA1(6121f0d5d08b343316123730624b3fd6a7c43c9e) )
	j6casclb_sound
ROM_END

ROM_START( j6casclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20061.bin", 0x000000, 0x020000, CRC(b9a3d52e) SHA1(2b2af88674a80d64073dec38992b2e5a76463389) )
	ROM_LOAD16_BYTE( "20060.bin", 0x000001, 0x020000, CRC(7da19bcc) SHA1(6121f0d5d08b343316123730624b3fd6a7c43c9e) )
	j6casclb_sound
ROM_END

ROM_START( j6ccc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "ccb1.bin", 0x00000, 0x020000, CRC(38a9b16e) SHA1(2ee0e1c67064e537b4459a69dc9ebf8be89f9051) )
	ROM_LOAD16_BYTE(  "ccb2.bin", 0x00001, 0x020000, CRC(5628a3e9) SHA1(79506828905e215fca410554a1dd1bac2050f11a) )
	j6casclb_sound
ROM_END

ROM_START( j6ccca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "ccbp1.bin", 0x0000, 0x020000, CRC(d337e556) SHA1(945461142b7e0b39f12c29c64713483aeed073ba) )
	ROM_LOAD16_BYTE(  "ccb2.bin", 0x00001, 0x020000, CRC(5628a3e9) SHA1(79506828905e215fca410554a1dd1bac2050f11a) )
	j6casclb_sound
ROM_END

ROM_START( j6cccb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "cce1.bin", 0x0000, 0x020000, CRC(0906747f) SHA1(ad9bb38843b81d0ef948b140448de1dab2058786) )
	ROM_LOAD16_BYTE(  "cce2.bin", 0x0001, 0x020000, CRC(33186e30) SHA1(8290c3128596a6e3d11e1af9c120f80defed5f97) )
	j6casclb_sound
ROM_END

ROM_START( j6cccc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "ccep1.bin", 0x0000, 0x020000, CRC(e2982047) SHA1(dc4991bb74358f3022e00ce81812687fa71d01ee) )
	ROM_LOAD16_BYTE(  "cce2.bin", 0x0001, 0x020000, CRC(33186e30) SHA1(8290c3128596a6e3d11e1af9c120f80defed5f97) )
	j6casclb_sound
ROM_END



#define j6caslas_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "clve-snd.bin", 0x000000, 0x020000, CRC(f77c12c7) SHA1(a2084be41949949aed787c1dccb64b969de81c86) ) /* looks like a bad dump, most missing */ \
	ROM_LOAD( "cas_las_vegas_snd.bin", 0x000000, 0x080000, CRC(d49c212e) SHA1(e4ddd743bdb9404afb5d6c3d2ae4ea1d625c331d) ) \
	ROM_LOAD( "caslasvegas30-06-97-4133as1.bin", 0x000000, 0x080000, CRC(d49c212e) SHA1(e4ddd743bdb9404afb5d6c3d2ae4ea1d625c331d) )


ROM_START( j6caslas )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9282.bin", 0x000000, 0x020000, CRC(a15cc050) SHA1(d08187db0577d28adb1b6cf62b040ee47d52c82a) )
	ROM_LOAD16_BYTE( "9283.bin", 0x000001, 0x020000, CRC(628fbb1e) SHA1(714a5077644c049d69837caf8e9ce6562fd2eb6c) )
	j6caslas_sound
ROM_END

ROM_START( j6caslasa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9284.bin", 0x000000, 0x020000, CRC(4ac29468) SHA1(6f511cb9420078c9a800f6758592756170369012) )
	ROM_LOAD16_BYTE( "9283.bin", 0x000001, 0x020000, CRC(628fbb1e) SHA1(714a5077644c049d69837caf8e9ce6562fd2eb6c) )
	j6caslas_sound
ROM_END

ROM_START( j6caslasb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9285.bin", 0x000000, 0x020000, CRC(f3980031) SHA1(f833f5301d644efaa8d4c82c953538c06733dc1b) )
	ROM_LOAD16_BYTE( "9283.bin", 0x000001, 0x020000, CRC(628fbb1e) SHA1(714a5077644c049d69837caf8e9ce6562fd2eb6c) )
	j6caslas_sound
ROM_END

ROM_START( j6caslasc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9286.bin", 0x000000, 0x020000, CRC(c331b1d8) SHA1(c3eefdc8001b9b3990537902f616a573cc13803a) )
	ROM_LOAD16_BYTE( "9283.bin", 0x000001, 0x020000, CRC(628fbb1e) SHA1(714a5077644c049d69837caf8e9ce6562fd2eb6c) )
	j6caslas_sound
ROM_END





ROM_START( j6cheque )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "chequemate.p1", 0x000000, 0x020000, CRC(108ac160) SHA1(0b3322975246325968d8966613927b395107775b) )
	ROM_LOAD16_BYTE( "chequemate.p2", 0x000001, 0x020000, CRC(425ad113) SHA1(9dae4056699547da3b1ddebbee7c2ae108bfae4d) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "chequematesnd.bin", 0x000000, 0x080000, CRC(310fc9a6) SHA1(636b6a0185ebe521441e01d7f381da630e31b3d9) )
ROM_END

#define j6cluclb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "clubcluedosnd.bin", 0x000000, 0x080000, CRC(80491c1b) SHA1(432ce61b26f77da10f5dc9230d8e3d4d988db4b4) )

ROM_START( j6cluclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clcl-c1.bin", 0x000000, 0x020000, CRC(5ca7cc7f) SHA1(58947a86f365f7daa3995f21a7846774dd03bccc) )
	ROM_LOAD16_BYTE( "clcl-c2.bin", 0x000001, 0x020000, CRC(cbb0f1f8) SHA1(af30a8c1ae0eabd2c3ee52dc5ae5c353be4ac4ee) )
	j6cluclb_sound
ROM_END


ROM_START( j6cluclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clcl-cb1.bin", 0x000000, 0x020000, CRC(50ea624e) SHA1(8bfc31f8e4e8ae1b56ea8443c94f00aa2f0c54d2) )
	ROM_LOAD16_BYTE( "clcl-c2.bin", 0x000001, 0x020000, CRC(cbb0f1f8) SHA1(af30a8c1ae0eabd2c3ee52dc5ae5c353be4ac4ee) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clcl-ce1.bin", 0x000000, 0x020000, CRC(443c901d) SHA1(fe8f0ee17369f5c268ab22669b559c2f5ddd9c55) )
	ROM_LOAD16_BYTE( "clcl-c2.bin", 0x000001, 0x020000, CRC(cbb0f1f8) SHA1(af30a8c1ae0eabd2c3ee52dc5ae5c353be4ac4ee) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clcl-cf1.bin", 0x000000, 0x020000, CRC(987d2f6f) SHA1(640c146610d52bf7119b770a7c6b98512f241434) )
	ROM_LOAD16_BYTE( "clcl-c2.bin", 0x000001, 0x020000, CRC(cbb0f1f8) SHA1(af30a8c1ae0eabd2c3ee52dc5ae5c353be4ac4ee) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clcl-cn1.bin", 0x000000, 0x020000, CRC(3ecabdf7) SHA1(81b085aace4e5847132e811585266986c6203c28) )
	ROM_LOAD16_BYTE( "clcl-c2.bin", 0x000001, 0x020000, CRC(cbb0f1f8) SHA1(af30a8c1ae0eabd2c3ee52dc5ae5c353be4ac4ee) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clcl-cp1.bin", 0x000000, 0x020000, CRC(b7399847) SHA1(21b536613c8a8a9df08c1dfeea2985e7353e5578) )
	ROM_LOAD16_BYTE( "clcl-c2.bin", 0x000001, 0x020000, CRC(cbb0f1f8) SHA1(af30a8c1ae0eabd2c3ee52dc5ae5c353be4ac4ee) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clclcnp1.bin", 0x000000, 0x020000, CRC(d554e9cf) SHA1(a987e60a7190d3ec616075f98caa8d821b7506c7) )
	ROM_LOAD16_BYTE( "clcl-c2.bin", 0x000001, 0x020000, CRC(cbb0f1f8) SHA1(af30a8c1ae0eabd2c3ee52dc5ae5c353be4ac4ee) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "b7df", 0x000000, 0x020000, CRC(cc06140e) SHA1(39da46d6dededad43b91802f88b19d22c99db1c9) )
	ROM_LOAD16_BYTE( "9204", 0x000001, 0x020000, CRC(08149887) SHA1(e6be7e4b129c636b8c5bbf86c818a3705363e610) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "b7ef", 0x000000, 0x020000, CRC(6ab18696) SHA1(36358c4b8dac26480bbf0a236b4b326bb2ee44a3) )
	ROM_LOAD16_BYTE( "9204", 0x000001, 0x020000, CRC(08149887) SHA1(e6be7e4b129c636b8c5bbf86c818a3705363e610) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "b7f7", 0x000000, 0x020000, CRC(39ea4fda) SHA1(d72ee50e81a9963ae90570d4d19b1cccf3641ee3) )
	ROM_LOAD16_BYTE( "9204", 0x000001, 0x020000, CRC(08149887) SHA1(e6be7e4b129c636b8c5bbf86c818a3705363e610) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "b7fb", 0x000000, 0x020000, CRC(1047ab7c) SHA1(23a1cc3a86b952592dc834f305e8faa7460ce474) )
	ROM_LOAD16_BYTE( "9204", 0x000001, 0x020000, CRC(08149887) SHA1(e6be7e4b129c636b8c5bbf86c818a3705363e610) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "b7fe", 0x000000, 0x020000, CRC(e342a326) SHA1(b59af7877378d9887e38e8b2d553dc6ddaad70ee) )
	ROM_LOAD16_BYTE( "9204", 0x000001, 0x020000, CRC(08149887) SHA1(e6be7e4b129c636b8c5bbf86c818a3705363e610) )
	j6cluclb_sound
ROM_END

ROM_START( j6cluclbl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "b7ff", 0x000000, 0x020000, CRC(08dcf71e) SHA1(97d7a7b482cd095c5455e889f512baa0056c2507) )
	ROM_LOAD16_BYTE( "9204", 0x000001, 0x020000, CRC(08149887) SHA1(e6be7e4b129c636b8c5bbf86c818a3705363e610) )
	j6cluclb_sound
ROM_END

#define j6col_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "coli_snd.bin", 0x000000, 0x080000, CRC(0f75b32e) SHA1(29e9e04d9a1686dfdf6cbdcd3acca23d6e64a048) )

ROM_START( j6col )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coliseum.p1", 0x000000, 0x020000, CRC(9830062b) SHA1(75865b49516b01754a8f63d5b33a08cd354dc3a6) )
	ROM_LOAD16_BYTE( "coliseum.p2", 0x000001, 0x020000, CRC(8b96052d) SHA1(558fb066f83c647668024f4e379c8ab150574d45) )
	j6col_sound
ROM_END

ROM_START( j6cola )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cseum1_0.p1", 0x000000, 0x020000, CRC(34d68c28) SHA1(a0a81bcea32226c4d9f98a02e455ee7cd8f01415) )
	ROM_LOAD16_BYTE( "cseum1_0.p2", 0x000001, 0x020000, CRC(75ebc601) SHA1(9a46491151adb020c2f5831b9bf8310e54fa319b) )
	j6col_sound
ROM_END


#define j6colcsh_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "coli-snd.bin", 0x000000, 0x080000, CRC(a194e5af) SHA1(649d6ecc4e15afd60c2f57d082a6d9846013c107) )
ROM_START( j6colcsh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-6_1.bin", 0x000000, 0x020000, CRC(e560f327) SHA1(48c619dc65e023621a530419ef6363865ff59c2a) )
	ROM_LOAD16_BYTE( "coli-6_2.bin", 0x000001, 0x020000, CRC(318c2ee5) SHA1(2ca39615775d92879353505b514dab9e3e63754b) )
	j6colcsh_sound
ROM_END

ROM_START( j6colcsha )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-6a1.bin", 0x000000, 0x020000, CRC(b7a43346) SHA1(7e21e73a8d44b020aeb82eb1f17fe5c220fa1467) )
	ROM_LOAD16_BYTE( "coli-6_2.bin", 0x000001, 0x020000, CRC(318c2ee5) SHA1(2ca39615775d92879353505b514dab9e3e63754b) )
	j6colcsh_sound
ROM_END

ROM_START( j6colcshb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-6n1.bin", 0x000000, 0x020000, CRC(d4564be3) SHA1(a13bd319c8d06d1e8c189becaaa84af15b8de443) )
	ROM_LOAD16_BYTE( "coli-6_2.bin", 0x000001, 0x020000, CRC(318c2ee5) SHA1(2ca39615775d92879353505b514dab9e3e63754b) )
	j6colcsh_sound
ROM_END

ROM_START( j6colcshc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-6p1.bin", 0x000000, 0x020000, CRC(0efea71f) SHA1(b0f5a5f59378e7c4f9ae3266d48f76c0eddbb567) )
	ROM_LOAD16_BYTE( "coli-6_2.bin", 0x000001, 0x020000, CRC(318c2ee5) SHA1(2ca39615775d92879353505b514dab9e3e63754b) )
	j6colcsh_sound
ROM_END

ROM_START( j6colcshd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli6np1.bin", 0x000000, 0x020000, CRC(3fc81fdb) SHA1(b3edd3c4fbe9e97fbcb2f43aa8d3518c6c5058fc) )
	ROM_LOAD16_BYTE( "coli-6_2.bin", 0x000001, 0x020000, CRC(318c2ee5) SHA1(2ca39615775d92879353505b514dab9e3e63754b) )
	j6colcsh_sound
ROM_END

ROM_START( j6colb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-4_1.bin", 0x000000, 0x020000, CRC(599d9e54) SHA1(37159694a31701a1d505e0d41b95c7d056d57e4c) ) // aka coliseum_awp.bin
	ROM_LOAD16_BYTE( "coli-4_2.bin", 0x000001, 0x020000, CRC(7636f9f2) SHA1(80f4d47b0171b98d7a4dc632562f95a3443ee95c) )
	j6colcsh_sound
ROM_END

ROM_START( j6colc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-4a1.bin", 0x000000, 0x020000, CRC(0b595e35) SHA1(f72d2d1d028333e954bedfc7139b37f33a85f2ff) )
	ROM_LOAD16_BYTE( "coli-4_2.bin", 0x000001, 0x020000, CRC(7636f9f2) SHA1(80f4d47b0171b98d7a4dc632562f95a3443ee95c) )
	j6colcsh_sound
ROM_END

ROM_START( j6cold )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-4n1.bin", 0x000000, 0x020000, CRC(68ab2690) SHA1(d07abd3cd19c0622a35a55eaf1e6ca34acfd0b98) )
	ROM_LOAD16_BYTE( "coli-4_2.bin", 0x000001, 0x020000, CRC(7636f9f2) SHA1(80f4d47b0171b98d7a4dc632562f95a3443ee95c) )
	j6colcsh_sound
ROM_END

ROM_START( j6cole )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-4np.bin", 0x000000, 0x020000, CRC(833572a8) SHA1(5d2a910180314bf28e186839d0e599dff5538cdd) )
	ROM_LOAD16_BYTE( "coli-4_2.bin", 0x000001, 0x020000, CRC(7636f9f2) SHA1(80f4d47b0171b98d7a4dc632562f95a3443ee95c) )
	j6colcsh_sound
ROM_END

ROM_START( j6colf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "coli-4p1.bin", 0x000000, 0x020000, CRC(b203ca6c) SHA1(07fddcaf9e7ea36f63873dda49507db29e0bb17a) )
	ROM_LOAD16_BYTE( "coli-4_2.bin", 0x000001, 0x020000, CRC(7636f9f2) SHA1(80f4d47b0171b98d7a4dc632562f95a3443ee95c) )
	j6colcsh_sound
ROM_END



#define j6colmon_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "colourofmoneysnd.bin", 0x000000, 0x080000, CRC(289d74c8) SHA1(04938d526e4a3079e9570fa946e5c6b9cc0ba311) )

ROM_START( j6colmon )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "colourofmoney.p1", 0x000000, 0x020000, CRC(d63f7a33) SHA1(414bff43b9a4e4ad387df01cd61980b5c2d696dd) )
	ROM_LOAD16_BYTE( "colourofmoney.p2", 0x000001, 0x020000, CRC(56e9ec06) SHA1(3bedfeadc7a321925057a77281794f7b50a9c46d) )
	j6colmon_sound
ROM_END

ROM_START( j6colmona )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-1_1.bin", 0x000000, 0x020000, CRC(5fcc5f83) SHA1(ad5389d2bf1cc6838a5427f804ae52e14f2c091b) )
	ROM_LOAD16_BYTE( "colourofmoney.p2", 0x000001, 0x020000, CRC(56e9ec06) SHA1(3bedfeadc7a321925057a77281794f7b50a9c46d) )
	j6colmon_sound
ROM_END

ROM_START( j6colmonb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-1a1.bin", 0x000000, 0x020000, CRC(0d089fe2) SHA1(93df5a9808039c82548e1a5248addac620c9f0ba) )
	ROM_LOAD16_BYTE( "colourofmoney.p2", 0x000001, 0x020000, CRC(56e9ec06) SHA1(3bedfeadc7a321925057a77281794f7b50a9c46d) )
	j6colmon_sound
ROM_END

ROM_START( j6colmonc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-1n1.bin", 0x000000, 0x020000, CRC(6efae747) SHA1(5753e21804dd3ab646b4ffb04b252c186e2ba99a) )
	ROM_LOAD16_BYTE( "colourofmoney.p2", 0x000001, 0x020000, CRC(56e9ec06) SHA1(3bedfeadc7a321925057a77281794f7b50a9c46d) )
	j6colmon_sound
ROM_END

ROM_START( j6colmond )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-1p1.bin", 0x000000, 0x020000, CRC(b4520bbb) SHA1(cd4533ad746d3306eaf20d3a169820e41d5c49d2) )
	ROM_LOAD16_BYTE( "colourofmoney.p2", 0x000001, 0x020000, CRC(56e9ec06) SHA1(3bedfeadc7a321925057a77281794f7b50a9c46d) )
	j6colmon_sound
ROM_END

ROM_START( j6colmone )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-1w1.bin", 0x000000, 0x020000, CRC(3da12e0b) SHA1(64ea48087e9926949137a916d9e396899eec409b) )
	ROM_LOAD16_BYTE( "colourofmoney.p2", 0x000001, 0x020000, CRC(56e9ec06) SHA1(3bedfeadc7a321925057a77281794f7b50a9c46d) )
	j6colmon_sound
ROM_END

ROM_START( j6colmonf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-2_1.bin", 0x000000, 0x020000, CRC(6ad59b66) SHA1(1fdd07e9ac46b6fdaabd5b98bcdeebff1e960efc) )
	ROM_LOAD16_BYTE( "como-2_2.bin", 0x000001, 0x020000, CRC(f37e7fc6) SHA1(421120f17afadb8bc806755f89562466ba39b386) )
	j6colmon_sound
ROM_END

ROM_START( j6colmong )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-2a1.bin", 0x000000, 0x020000, CRC(38115b07) SHA1(fd15616ea2a6c88337ee7891f29d351c596f2bc5) )
	ROM_LOAD16_BYTE( "como-2_2.bin", 0x000001, 0x020000, CRC(f37e7fc6) SHA1(421120f17afadb8bc806755f89562466ba39b386) )
	j6colmon_sound
ROM_END

ROM_START( j6colmonh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-2n1.bin", 0x000000, 0x020000, CRC(5be323a2) SHA1(1fae13b25d9acc70047779a22fe4cd36e2d532cd) )
	ROM_LOAD16_BYTE( "como-2_2.bin", 0x000001, 0x020000, CRC(f37e7fc6) SHA1(421120f17afadb8bc806755f89562466ba39b386) )
	j6colmon_sound
ROM_END

ROM_START( j6colmoni )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como-2p1.bin", 0x000000, 0x020000, CRC(814bcf5e) SHA1(3a2147cbe9d97eed4f90373f1323a1ed61a0c9d8) )
	ROM_LOAD16_BYTE( "como-2_2.bin", 0x000001, 0x020000, CRC(f37e7fc6) SHA1(421120f17afadb8bc806755f89562466ba39b386) )
	j6colmon_sound
ROM_END

ROM_START( j6colmonj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "como2np1.bin", 0x000000, 0x020000, CRC(b07d779a) SHA1(101617eb7ded22b28cb19c91eb736b9a7dc91369) )
	ROM_LOAD16_BYTE( "como-2_2.bin", 0x000001, 0x020000, CRC(f37e7fc6) SHA1(421120f17afadb8bc806755f89562466ba39b386) )
	j6colmon_sound
ROM_END


#define j6coprob_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "crsound.bin", 0x000000, 0x080000, CRC(d39dd4f1) SHA1(9ce7870c00f9ccb797182af41a7d22e41624c8ce) )

ROM_START( j6coprob )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60000024.p1", 0x000000, 0x020000, CRC(7ada6793) SHA1(9e2aa2e033bc6535d39caffe435ac6a7cc57d4f9) )
	ROM_LOAD16_BYTE( "60000024.p2", 0x000001, 0x020000, CRC(4b89f47b) SHA1(e2e5c2242afd4d8c338d40cf8b3da58ff876d2e3) )
	j6coprob_sound
ROM_END

ROM_START( j6coproba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60001024.p1", 0x000000, 0x020000, CRC(75491f8f) SHA1(b905488d6742b8ae2eefdb7f8b99476c3d839ab7) )
	ROM_LOAD16_BYTE( "60001024.p2", 0x000001, 0x020000, CRC(9b154ee7) SHA1(5be113031c877c407f06fa48d4565c6f4ddd4961) )
	j6coprob_sound
ROM_END

ROM_START( j6coprobb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cnr_5f8c_v200.p1", 0x000000, 0x020000, CRC(ce95a60f) SHA1(3661c680eebb2fcda7482255168a60b8224870f1) )
	ROM_LOAD16_BYTE( "cnr_d1cf_v200.p2", 0x000001, 0x020000, CRC(88e0bb1c) SHA1(c70d39d918cfc7b5c91328fb52017a6899c0c602) )
	j6coprob_sound
ROM_END

ROM_START( j6coprobc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cnr_a9e3_v200.p1", 0x000000, 0x020000, CRC(3103d119) SHA1(087221127ed533e64377c4704bae659a82cbd97e) )
	ROM_LOAD16_BYTE( "cnr_dbea_v200.p2", 0x000001, 0x020000, CRC(ca6960c8) SHA1(4224e6a6bece51dce698f40f795bafdc4e92d4a4) )
	j6coprob_sound
ROM_END

ROM_START( j6coprobd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cnr_6a28_v200.p1", 0x000000, 0x020000, CRC(54c790d1) SHA1(d14e03af85bbccb3160243e91a2c8c95901f645a) )
	ROM_LOAD16_BYTE( "cnr_fdc0_v200.p2", 0x000001, 0x020000, CRC(2109c6d7) SHA1(2c72231feca1888dd3a1d69081d8057d3847ce4f) )
	j6coprob_sound
ROM_END

ROM_START( j6coprobe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cnr_v200.p1", 0x000000, 0x020000, CRC(8d99ddd3) SHA1(465dfeb5316050c44f9a6c062da12bf62ab626d0) )
	ROM_LOAD16_BYTE( "cnr_v200.p2", 0x000001, 0x020000, CRC(81326a7f) SHA1(99f71768aaf49edaf20c3e72b18caea21df8686f) )
	j6coprob_sound
ROM_END


#define j6crack_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "crck_snd.hex", 0x000000, 0x080000, CRC(106bb6b5) SHA1(a2e109fd71575dcceec190efed0a9c81ec1f4048) )

ROM_START( j6crack )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-2_1.bin", 0x000000, 0x020000, CRC(519116d1) SHA1(a99342eb7644ade2f904a148b13ea6e81c96d8ec) )
	ROM_LOAD16_BYTE( "crac-2_2.bin", 0x000001, 0x020000, CRC(60a9914a) SHA1(d5f8dad456e36cb455b0f5278b0f38edc78e4b49) )
	j6crack_sound
ROM_END

ROM_START( j6cracka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-2a1.bin", 0x000000, 0x020000, CRC(0355d6b0) SHA1(7a929131720c8f82c9efaac005641a54f31f6758) )
	ROM_LOAD16_BYTE( "crac-2_2.bin", 0x000001, 0x020000, CRC(60a9914a) SHA1(d5f8dad456e36cb455b0f5278b0f38edc78e4b49) )
	j6crack_sound
ROM_END

ROM_START( j6crackb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-2n1.bin", 0x000000, 0x020000, CRC(60a7ae15) SHA1(3fd13083775348871be6afae7dd82adf1143af1c) )
	ROM_LOAD16_BYTE( "crac-2_2.bin", 0x000001, 0x020000, CRC(60a9914a) SHA1(d5f8dad456e36cb455b0f5278b0f38edc78e4b49) )
	j6crack_sound
ROM_END

ROM_START( j6crackc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-2np.bin", 0x000000, 0x020000, CRC(8b39fa2d) SHA1(12e76c767ccfe08d685933ea7734cdd3aef763c7) )
	ROM_LOAD16_BYTE( "crac-2_2.bin", 0x000001, 0x020000, CRC(60a9914a) SHA1(d5f8dad456e36cb455b0f5278b0f38edc78e4b49) )
	j6crack_sound
ROM_END

ROM_START( j6crackd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-2p1.bin", 0x000000, 0x020000, CRC(ba0f42e9) SHA1(24db750b30e6ed5cc29b4e621a2be1ad1a514506) )
	ROM_LOAD16_BYTE( "crac-2_2.bin", 0x000001, 0x020000, CRC(60a9914a) SHA1(d5f8dad456e36cb455b0f5278b0f38edc78e4b49) )
	j6crack_sound
ROM_END

ROM_START( j6cracke )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-2w1.bin", 0x000000, 0x020000, CRC(33fc6759) SHA1(f0c8136093196870d188d90c334c0f3197df7f4f) )
	ROM_LOAD16_BYTE( "crac-2_2.bin", 0x000001, 0x020000, CRC(60a9914a) SHA1(d5f8dad456e36cb455b0f5278b0f38edc78e4b49) )
	j6crack_sound
ROM_END

ROM_START( j6crackf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-3_1.bin", 0x000000, 0x020000, CRC(94d59bb4) SHA1(04a62de8ea746009d9c658e37b1d4900645f942f) )
	ROM_LOAD16_BYTE( "crac-3_2.bin", 0x000001, 0x020000, CRC(d2d8fb65) SHA1(4014e3f48c7b800c5232fda2b48023a3d56b4150) )
	j6crack_sound
ROM_END

ROM_START( j6crackg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-3a1.bin", 0x000000, 0x020000, CRC(c6115bd5) SHA1(bd926b903afcf665c66aa5b6a65e1b1a8d4155fc) )
	ROM_LOAD16_BYTE( "crac-3_2.bin", 0x000001, 0x020000, CRC(d2d8fb65) SHA1(4014e3f48c7b800c5232fda2b48023a3d56b4150) )
	j6crack_sound
ROM_END

ROM_START( j6crackh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-3n1.bin", 0x000000, 0x020000, CRC(a5e32370) SHA1(2c64fd2c3fadb2e882ed5edb0009e70ec16d62ad) )
	ROM_LOAD16_BYTE( "crac-3_2.bin", 0x000001, 0x020000, CRC(d2d8fb65) SHA1(4014e3f48c7b800c5232fda2b48023a3d56b4150) )
	j6crack_sound
ROM_END

ROM_START( j6cracki )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac-3p1.bin", 0x000000, 0x020000, CRC(7f4bcf8c) SHA1(170e2f3cf206282ab51b1872809f92676f1ffda5) )
	ROM_LOAD16_BYTE( "crac-3_2.bin", 0x000001, 0x020000, CRC(d2d8fb65) SHA1(4014e3f48c7b800c5232fda2b48023a3d56b4150) )
	j6crack_sound
ROM_END

ROM_START( j6crackj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "crac3np1.bin", 0x000000, 0x020000, CRC(4e7d7748) SHA1(c1d2e085b0b239a5217ca29e50f88caa5b24dcd3) )
	ROM_LOAD16_BYTE( "crac-3_2.bin", 0x000001, 0x020000, CRC(d2d8fb65) SHA1(4014e3f48c7b800c5232fda2b48023a3d56b4150) )
	j6crack_sound
ROM_END


#define j6crzclb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6crzclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "01ff.p1", 0x000000, 0x020000, CRC(5c9ad223) SHA1(4b8ffe604f0a71b8b7a63919a1dceaab92a2e7d4) )
	ROM_LOAD16_BYTE( "1f83.p2", 0x000001, 0x020000, CRC(5dfa134a) SHA1(4127cb36f2c5a6a1ce0a5023f8064bffb29c4799) )
	j6crzclb_sound
ROM_END

ROM_START( j6crzclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "01df", 0x000000, 0x020000, CRC(98403133) SHA1(b43c964c02a5aa307540541bd45bfea9456571c4) )
	ROM_LOAD16_BYTE( "1f83.p2", 0x000001, 0x020000, CRC(5dfa134a) SHA1(4127cb36f2c5a6a1ce0a5023f8064bffb29c4799) )
	j6crzclb_sound
ROM_END

ROM_START( j6crzclbb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "01f7", 0x000000, 0x020000, CRC(6dac6ae7) SHA1(8868f6eadbbb12704ef7ee5fff52569f19a628d9) )
	ROM_LOAD16_BYTE( "1f83.p2", 0x000001, 0x020000, CRC(5dfa134a) SHA1(4127cb36f2c5a6a1ce0a5023f8064bffb29c4799) )
	j6crzclb_sound
ROM_END

ROM_START( j6crzclbc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "01fe", 0x000000, 0x020000, CRC(b704861b) SHA1(6e4c34d9a18c1db99e900bd9dc17270587c96fde) )
	ROM_LOAD16_BYTE( "1f83.p2", 0x000001, 0x020000, CRC(5dfa134a) SHA1(4127cb36f2c5a6a1ce0a5023f8064bffb29c4799) )
	j6crzclb_sound
ROM_END


#define j6crsfir_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "crossfiresnd.bin", 0x000000, 0x080000, CRC(266edecb) SHA1(c985081bd2a4500889aae0dc7ecd7d8e4cbd1591) )

ROM_START( j6crsfir )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cf30cz_05_1.b8", 0x000000, 0x020000, CRC(1b65c071) SHA1(972626f73b3c7fdce97aae913ceea523f70a6ccc) )
	ROM_LOAD16_BYTE( "cf30cz_05_2.b8", 0x000001, 0x020000, CRC(dc630ed6) SHA1(9f3370126ceca05f21ebc13bba4d9efea7cb8b46) ) // aka cf75cz_05_2.b8
	j6crsfir_sound
ROM_END


ROM_START( j6crsfira )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "cf75cz_05_1.b8", 0x000000, 0x020000, CRC(17286e40) SHA1(979c02e6005705b258c04c1e38ad8d0896b61b3d) )
	ROM_LOAD16_BYTE( "cf30cz_05_2.b8", 0x000001, 0x020000, CRC(dc630ed6) SHA1(9f3370126ceca05f21ebc13bba4d9efea7cb8b46) ) // aka cf75cz_05_2.b8
	j6crsfir_sound
ROM_END

#define j6daygld_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* dgsnd.bin = popsnd.bin            m_popeye   Popeye (20p/8 GBP Token) */ \
	ROM_LOAD( "dgsnd2.b8", 0x000000, 0x080000, CRC(7e8c05ce) SHA1(616b0f94b94331f86d7b1fec11dd05cf9b0968cf) )
ROM_START( j6daygld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dg30cz04_1.b8", 0x000000, 0x020000, CRC(ff3aefc9) SHA1(80e5485f0f6e67dac3bfaa5b3ba947ff63f9b745) )
	ROM_LOAD16_BYTE( "dg30cz04_2.b8", 0x000001, 0x020000, CRC(abedad68) SHA1(4a49695527ab0441e9c16cef9c632f6ac74d2d9a) )
	j6daygld_sound
ROM_END

ROM_START( j6dayglda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dg30cz05_1.b8", 0x000000, 0x020000, CRC(e6808c66) SHA1(e0a6513ca62299c0906e458e84de46b3475e8f85) )
	ROM_LOAD16_BYTE( "dg30cz05_2.b8", 0x000001, 0x020000, CRC(a4d2f2cd) SHA1(77890d29c50ef9a355de898ef54a214b68c155c1) )
	j6daygld_sound
ROM_END

ROM_START( j6daygldb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dg75cz05_1.b8", 0x000000, 0x020000, CRC(eacd2257) SHA1(1c116944e5964d8d2b665e629c16cac61bc9fe9b) )
	ROM_LOAD16_BYTE( "dg30cz05_2.b8", 0x000001, 0x020000, CRC(a4d2f2cd) SHA1(77890d29c50ef9a355de898ef54a214b68c155c1) )
	j6daygld_sound
ROM_END

ROM_START( j6daygldc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dg30cz9_1.b8", 0x000000, 0x020000, CRC(03ec9247) SHA1(65d5d9f287da6fa8c832215b278033c65b62398a) )
	ROM_LOAD16_BYTE( "dg9_2.b8", 0x000001, 0x020000, CRC(7214a595) SHA1(bbaea30fc89afdf981536592016fcb4589447d7b) )
	j6daygld_sound
ROM_END

ROM_START( j6daygldd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dg75cz9_1.b8", 0x000000, 0x020000, CRC(0fa13c76) SHA1(e2ba888057f6b0ddafc47e57b428f548c3d45bb8) )
	ROM_LOAD16_BYTE( "dg9_2.b8", 0x000001, 0x020000, CRC(7214a595) SHA1(bbaea30fc89afdf981536592016fcb4589447d7b) )
	j6daygld_sound
ROM_END

ROM_START( j6dayglde )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dg75cz02_1.b8", 0x000000, 0x020000, CRC(2c73b140) SHA1(d1172f435abaad65ba7c3c8e8401bd88284caca5) ) // aka dg75cza.bin
	ROM_LOAD16_BYTE( "dg75cz02_2.b8", 0x000001, 0x020000, CRC(f400645b) SHA1(59fdb302b421398985989bda02ee82b4e36b51d1) ) // aka dg75czb.bin
	j6daygld_sound
ROM_END


#define j6dayml_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "daytona-sound_4m.epr", 0x000000, 0x080000, CRC(a74fa29c) SHA1(3db3322910717d4eda81b5df5988453fdebec7bf) )
ROM_START( j6dayml )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dm30cz08_1.b8", 0x000000, 0x020000, CRC(488a9da7) SHA1(604a2a6f7f07a9506906abeea79066459f375e61) )
	ROM_LOAD16_BYTE( "dm30cz08_2.b8", 0x000001, 0x020000, CRC(be52b8c0) SHA1(54e7a936d94cf65e246ddf6458e9d52c5b44cb8e) )
	j6dayml_sound
ROM_END

ROM_START( j6daymla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dm75cz08_1.b8", 0x000000, 0x020000, CRC(44c73396) SHA1(70ea5927a56bc44f4abb1b6d870c05935169c2d5) )
	ROM_LOAD16_BYTE( "dm30cz08_2.b8", 0x000001, 0x020000, CRC(be52b8c0) SHA1(54e7a936d94cf65e246ddf6458e9d52c5b44cb8e) )
	j6dayml_sound
ROM_END

ROM_START( j6daymlb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dymil751.epr", 0x000000, 0x020000, CRC(99a735bd) SHA1(100fcaed9eca910424fd0872d03d9a594f96e0b5) )
	ROM_LOAD16_BYTE( "dymil752.epr", 0x000001, 0x020000, CRC(e6f2770d) SHA1(15d0ec40e31dc03c271774c873bc9651bd35336a) )
	j6dayml_sound
ROM_END


#define j6dmnjkr_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "demonjocker-sound_4m.epr", 0x000000, 0x04a018, CRC(796e1b35) SHA1(e9c8e5a350823275c9ba9238781872ea359d5049) )

ROM_START( j6dmnjkr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "demonjok_73001.b8", 0x000000, 0x020000, CRC(83670696) SHA1(d98713d54e42002ca2ba9b5c80a671121342400e) )
	ROM_LOAD16_BYTE( "demonjok_73002.b8", 0x000001, 0x020000, CRC(2a7658ab) SHA1(4286a4a76b8d95a4da4e8aad2f81b091d2d2f96a) ) // aka demonjok_77502.b8
	j6dmnjkr_sound
ROM_END

ROM_START( j6dmnjkra )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "demonjok_77501.b8", 0x000000, 0x020000, CRC(8f2aa8a7) SHA1(ea5b8c1418deaaf2bda58a40cacca3c77d6d5a08) ) // j6reddmn
	ROM_LOAD16_BYTE( "demonjok_73002.b8", 0x000001, 0x020000, CRC(2a7658ab) SHA1(4286a4a76b8d95a4da4e8aad2f81b091d2d2f96a) ) // j6reddmn - aka demonjok_77502.b8
	j6dmnjkr_sound
ROM_END

ROM_START( j6dmnjkrb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dj30cz08_1.b8", 0x000000, 0x020000, CRC(b0470eae) SHA1(fe1c8e613083740c09be1703f7424fa9907a850a) )
	ROM_LOAD16_BYTE( "dj30cz08_2.b8", 0x000001, 0x020000, CRC(ba4aba56) SHA1(8c9ad521e35fc83ff52974622c26c537961abab3) )
	j6dmnjkr_sound
ROM_END

ROM_START( j6dmnjkrc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dj75cz08_1.b8", 0x000000, 0x020000, CRC(bc0aa09f) SHA1(f8de115fdc6bc0fca24590822f0f14acbfcdb23c) )
	ROM_LOAD16_BYTE( "dj30cz08_2.b8", 0x000001, 0x020000, CRC(ba4aba56) SHA1(8c9ad521e35fc83ff52974622c26c537961abab3) )
	j6dmnjkr_sound
ROM_END

/* the same as j6dmnjkra?? alt sound rom */
ROM_START( j6reddmn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "reddemon1.bin", 0x000000, 0x020000, CRC(8f2aa8a7) SHA1(ea5b8c1418deaaf2bda58a40cacca3c77d6d5a08) )
	ROM_LOAD16_BYTE( "reddemon2.bin", 0x000001, 0x020000, CRC(2a7658ab) SHA1(4286a4a76b8d95a4da4e8aad2f81b091d2d2f96a) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "reddemonsnd.bin", 0x000000, 0x080000, CRC(4fb1cbff) SHA1(27393e14af18f05df07bcbbab957a684de79dbb1) )
ROM_END

/* the same as j6dmnjkra?? */
ROM_START( j6gldday )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "goldenday1.bin", 0x000000, 0x020000, CRC(8f2aa8a7) SHA1(ea5b8c1418deaaf2bda58a40cacca3c77d6d5a08) )
	ROM_LOAD16_BYTE( "goldenday2.bin", 0x000001, 0x020000, CRC(2a7658ab) SHA1(4286a4a76b8d95a4da4e8aad2f81b091d2d2f96a) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "gdsound.bin", 0x000000, 0x04a018, CRC(796e1b35) SHA1(e9c8e5a350823275c9ba9238781872ea359d5049) )
ROM_END

ROM_START( j6dmngz )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "diamondgeezer.p1", 0x000000, 0x020000, CRC(518cd4e2) SHA1(93810c41f264666d0683179779f35dbbf3b86aa3) )
	ROM_LOAD16_BYTE( "diamondgeezer.p2", 0x000001, 0x020000, CRC(24ac143a) SHA1(e25f64021b09f12a5d0f8146bf071f6e565e0ba5) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "diamondgeezersnd.bin", 0x000000, 0x080000, CRC(6aa61ba4) SHA1(173b7aa31da9a9ee322653634711283f602b2743) )
ROM_END

#define j6dyfl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "doyoufeelluckysnd.bin", 0x000000, 0x080000, CRC(9148112e) SHA1(8976a03eb68b1f08c6260c095bddb8d8731539af) )

ROM_START( j6dyfl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-2_1.bin", 0x000000, 0x020000, CRC(4bd437c8) SHA1(70458993498b3803a723deb7e519708c6860d6cf) )
	ROM_LOAD16_BYTE( "dyfl-2_2.bin", 0x000001, 0x020000, CRC(25b31fc7) SHA1(1af3b14fd42de9db7834b2973f223cad282eb74d) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyfla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-2a1.bin", 0x000000, 0x020000, CRC(1910f7a9) SHA1(2200a1ed2dea0e2c486885ae19ef918e709af244) )
	ROM_LOAD16_BYTE( "dyfl-2_2.bin", 0x000001, 0x020000, CRC(25b31fc7) SHA1(1af3b14fd42de9db7834b2973f223cad282eb74d) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyflb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-2n1.bin", 0x000000, 0x020000, CRC(7ae28f0c) SHA1(fd3bf97fcf485fa34e2c8cace5f2880643dedb49) )
	ROM_LOAD16_BYTE( "dyfl-2_2.bin", 0x000001, 0x020000, CRC(25b31fc7) SHA1(1af3b14fd42de9db7834b2973f223cad282eb74d) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyflc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-2np.bin", 0x000000, 0x020000, CRC(917cdb34) SHA1(931ec1ab8bb675f549adc3169d6379f83f11726f) )
	ROM_LOAD16_BYTE( "dyfl-2_2.bin", 0x000001, 0x020000, CRC(25b31fc7) SHA1(1af3b14fd42de9db7834b2973f223cad282eb74d) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyfld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-2p1.bin", 0x000000, 0x020000, CRC(a04a63f0) SHA1(40c46d0e121e340000417aa07e8adba229dba499) )
	ROM_LOAD16_BYTE( "dyfl-2_2.bin", 0x000001, 0x020000, CRC(25b31fc7) SHA1(1af3b14fd42de9db7834b2973f223cad282eb74d) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyfle )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-2w1.bin", 0x000000, 0x020000, CRC(29b94640) SHA1(d8608d8f3d5def75b6a4fa0b04e7d1604abc86ea) )
	ROM_LOAD16_BYTE( "dyfl-2_2.bin", 0x000001, 0x020000, CRC(25b31fc7) SHA1(1af3b14fd42de9db7834b2973f223cad282eb74d) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyflf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-3_1.bin", 0x000000, 0x020000, CRC(98242812) SHA1(ed2e678c4864d7f2b544e161ef2515a1a21178c2) )
	ROM_LOAD16_BYTE( "dyfl-3_2.bin", 0x000001, 0x020000, CRC(66e2a419) SHA1(969a1c61f5be9acf2d3a345c2ade59438e648f4a) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyflg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-3a1.bin", 0x000000, 0x020000, CRC(cae0e873) SHA1(cee90a05f04d3fbba2130728adda5051a7886c1f) )
	ROM_LOAD16_BYTE( "dyfl-3_2.bin", 0x000001, 0x020000, CRC(66e2a419) SHA1(969a1c61f5be9acf2d3a345c2ade59438e648f4a) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyflh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-3n1.bin", 0x000000, 0x020000, CRC(a91290d6) SHA1(f703e280a3c87c5e0c1dc9386b723b4194305894) )
	ROM_LOAD16_BYTE( "dyfl-3_2.bin", 0x000001, 0x020000, CRC(66e2a419) SHA1(969a1c61f5be9acf2d3a345c2ade59438e648f4a) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyfli )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl-3p1.bin", 0x000000, 0x020000, CRC(73ba7c2a) SHA1(a48209d5322e49788d3657d80426cc66f8092b12) )
	ROM_LOAD16_BYTE( "dyfl-3_2.bin", 0x000001, 0x020000, CRC(66e2a419) SHA1(969a1c61f5be9acf2d3a345c2ade59438e648f4a) )
	j6dyfl_sound
ROM_END

ROM_START( j6dyflj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "dyfl3np1.bin", 0x000000, 0x020000, CRC(428cc4ee) SHA1(d49b4bb84f036d154bdee723925a26278253441b) )
	ROM_LOAD16_BYTE( "dyfl-3_2.bin", 0x000001, 0x020000, CRC(66e2a419) SHA1(969a1c61f5be9acf2d3a345c2ade59438e648f4a) )
	j6dyfl_sound
ROM_END





ROM_START( j6drdogh ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "drdough.p1", 0x000000, 0x080000, CRC(e669d9eb) SHA1(0ba5a02f0ba34f79ca86536ea04908ea6bd63b99) )
	ROM_LOAD16_BYTE( "drdough.p2", 0x000001, 0x080000, CRC(400ba359) SHA1(3c2a0f9df9fd81a011f82fcc2b4f9a9fe850ee9c) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "drdoughsnd.bin", 0x000000, 0x080000, CRC(491676a5) SHA1(967841bf033aceead84db2e40acdd5f2e7e2b1e9) )
ROM_END

#define j6euphor_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "euphoriasnd.bin", 0x000000, 0x080000, CRC(d3097d34) SHA1(3db500b5ee38dfa580336b4bac43b139a31d2638) )

ROM_START( j6euphor )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "euph-1a1.bin", 0x000000, 0x020000, CRC(cb88f5e6) SHA1(247250040b44f4129e35bc3a77eb42e4cb14d8bd) )
	ROM_LOAD16_BYTE( "euph-1a2.bin", 0x000001, 0x020000, CRC(75c3deeb) SHA1(02d045c2fe2977195d02807f8f8576d2a9d48b63) )
	j6euphor_sound
ROM_END

ROM_START( j6euphora )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "euph-11.bin", 0x000000, 0x020000, CRC(2016a1de) SHA1(f11182b5903696f4a1e206a975fd7552e25baaf0) )
	ROM_LOAD16_BYTE( "euph-1a2.bin", 0x000001, 0x020000, CRC(75c3deeb) SHA1(02d045c2fe2977195d02807f8f8576d2a9d48b63) )
	j6euphor_sound
ROM_END

ROM_START( j6euphorb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "euph-1n1.bin", 0x000000, 0x020000, CRC(a87a8d43) SHA1(0f2619d84a47c4e10895d228a770ff3a178aabca) )
	ROM_LOAD16_BYTE( "euph-1a2.bin", 0x000001, 0x020000, CRC(75c3deeb) SHA1(02d045c2fe2977195d02807f8f8576d2a9d48b63) )
	j6euphor_sound
ROM_END

ROM_START( j6euphorc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "euph-1p1.bin", 0x000000, 0x020000, CRC(72d261bf) SHA1(7fcc2e9fc17742e4c43360ca8ffb1f45b1a3dbf4) )
	ROM_LOAD16_BYTE( "euph-1a2.bin", 0x000001, 0x020000, CRC(75c3deeb) SHA1(02d045c2fe2977195d02807f8f8576d2a9d48b63) )
	j6euphor_sound
ROM_END

ROM_START( j6euphord )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "euph11.bin", 0x000000, 0x020000, CRC(994c3587) SHA1(013f98d5d1ca6927628cbd63ad7f4de12ec009a5) )
	ROM_LOAD16_BYTE( "euph-1a2.bin", 0x000001, 0x020000, CRC(75c3deeb) SHA1(02d045c2fe2977195d02807f8f8576d2a9d48b63) )
	j6euphor_sound
ROM_END

ROM_START( j6euphore )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "euph1np1.bin", 0x000000, 0x020000, CRC(43e4d97b) SHA1(5b95539b84a5688dc1706d53e150b962494ac924) )
	ROM_LOAD16_BYTE( "euph-1a2.bin", 0x000001, 0x020000, CRC(75c3deeb) SHA1(02d045c2fe2977195d02807f8f8576d2a9d48b63) )
	j6euphor_sound
ROM_END

ROM_START( j6euphorf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "euphoria.p1", 0x00000, 0x020000, CRC(27d40f99) SHA1(5732aa3b8d0e67acef3dda640453ad60caec1bcb) )
	ROM_LOAD16_BYTE( "euphoria.p2", 0x00001, 0x020000, CRC(480b239c) SHA1(9d0cdb979b2f63bdf5eef2d8b7a4718100b0c1bd) )
	j6euphor_sound
ROM_END

#define j6fastfr_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cffsnd.bin", 0x000000, 0x080000, CRC(29355a37) SHA1(5810f0eafe58b5d03cd104381eb92f55b1e08baa) )

ROM_START( j6fastfr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clubfastfruits_v200.p1", 0x000000, 0x020000, CRC(7eef851a) SHA1(1ae689c08ff977644d8a0820e8f1c5dc82806939) )
	ROM_LOAD16_BYTE( "clubfastfruits_v200.p2", 0x000001, 0x020000, CRC(477a5c1a) SHA1(04db692a2b8258c5b8404a9143836c681ca43544) )
	j6fastfr_sound
ROM_END


ROM_START( j6fastfra )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60003002.evn", 0x000000, 0x020000, CRC(18edd2fc) SHA1(eaf890328d90fe1f80a254877d2bc276da10a16f) )
	ROM_LOAD16_BYTE( "60003002.odd", 0x000001, 0x020000, CRC(97b39bc2) SHA1(9edccd3a4ecf62c04620e64350ba517aa086b2b1) )
	j6fastfr_sound
ROM_END


#define j6fasttk_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "fasttrak.upd", 0x000000, 0x080000, NO_DUMP )

ROM_START( j6fasttk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9331.bin", 0x000000, 0x020000, CRC(54dbf894) SHA1(a3ffff82883cc192108f44d36a7465d4afeaf114) )
	ROM_LOAD16_BYTE( "9332.bin", 0x000001, 0x020000, CRC(ecf1632a) SHA1(5d82a46672adceb29744e82de1b0fa5fcf4dbc51) )
	j6fasttk_sound
ROM_END

ROM_START( j6fasttka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9333.bin", 0x000000, 0x020000, CRC(bf45acac) SHA1(ec624bc2d135901ecbdb6c6b3dbd9cc4b618b4de) )
	ROM_LOAD16_BYTE( "9332.bin", 0x000001, 0x020000, CRC(ecf1632a) SHA1(5d82a46672adceb29744e82de1b0fa5fcf4dbc51) )
	j6fasttk_sound
ROM_END

ROM_START( j6fasttkb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9334.bin", 0x000000, 0x020000, CRC(061f38f5) SHA1(459b39d2380fcfdb763eeb6937752be192cb8244) )
	ROM_LOAD16_BYTE( "9332.bin", 0x000001, 0x020000, CRC(ecf1632a) SHA1(5d82a46672adceb29744e82de1b0fa5fcf4dbc51) )
	j6fasttk_sound
ROM_END

ROM_START( j6fasttkc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9335.bin", 0x000000, 0x020000, CRC(36b6891c) SHA1(013b663f2dc59a4d2834ef2f7e86bcc608e98b39) )
	ROM_LOAD16_BYTE( "9332.bin", 0x000001, 0x020000, CRC(ecf1632a) SHA1(5d82a46672adceb29744e82de1b0fa5fcf4dbc51) )
	j6fasttk_sound
ROM_END


#define j6filth_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1151.bin", 0x000000, 0x080000, CRC(c3a2bf9b) SHA1(31536613fd9dcce0878109d460344591570c4334) )

ROM_START( j6filth )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7729.bin", 0x000000, 0x020000, CRC(5f272354) SHA1(23d2c710a628af9731ea67877ffd7b8309469c09) )
	ROM_LOAD16_BYTE( "7730.bin", 0x000001, 0x020000, CRC(83bbd350) SHA1(2171c3ddec8787b7ee0b48a022046490ebcf3bf9) )
	j6filth_sound
ROM_END

ROM_START( j6filtha )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8198.bin", 0x000000, 0x020000, CRC(c77862d7) SHA1(33befc3976e68016489ce773fa964974fe62a8ce) )
	ROM_LOAD16_BYTE( "8199.bin", 0x000001, 0x020000, CRC(58a07ea1) SHA1(c33c464b2646e058a3f3013922298ed7ee3a3d67) )
	j6filth_sound
ROM_END

ROM_START( j6filthb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8200.bin", 0x000000, 0x020000, CRC(2ce636ef) SHA1(c8980f5b4e7786f103d24effa7f258bfddc1e7b2) )
	ROM_LOAD16_BYTE( "8199.bin", 0x000001, 0x020000, CRC(58a07ea1) SHA1(c33c464b2646e058a3f3013922298ed7ee3a3d67) )
	j6filth_sound
ROM_END

ROM_START( j6filthc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8201.bin", 0x000000, 0x020000, CRC(03a281c7) SHA1(fb5adbc3dafda25b4133730c0fff800014e295af) )
	ROM_LOAD16_BYTE( "8199.bin", 0x000001, 0x020000, CRC(58a07ea1) SHA1(c33c464b2646e058a3f3013922298ed7ee3a3d67) )
	j6filth_sound
ROM_END



#define j6firbl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "fireballsnd.bin", 0x000000, 0x080000, CRC(e47444c7) SHA1(535ae2abdf5f9a1931c8b2afccf9a63b0778e5e3) )

ROM_START( j6firbl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fireball.p1", 0x000000, 0x020000, CRC(c20a33dd) SHA1(4489b796d3b0121fbbeb9e226200566c0467dab6) )
	ROM_LOAD16_BYTE( "fireball.p2", 0x000001, 0x020000, CRC(fcc66f23) SHA1(15d4b65300377734692553edd58863627500eb41) )
	j6firbl_sound
ROM_END

ROM_START( j6firbla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fire-2_1.bin", 0x000000, 0x020000, CRC(a1e44b18) SHA1(871a7c8c85c31c46c885737a7dc2d5ea38301c25) )
	ROM_LOAD16_BYTE( "fire-2_2.bin", 0x000001, 0x020000, CRC(ef3ac4d2) SHA1(3cbae5282f6c5adb75c7bdc76f93350671389155) )
	j6firbl_sound
ROM_END

ROM_START( j6firblb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fire-2a1.bin", 0x000000, 0x020000, CRC(f3208b79) SHA1(1014ba090ba8dc5968fc2069e20c8a89c70f8038) )
	ROM_LOAD16_BYTE( "fire-2_2.bin", 0x000001, 0x020000, CRC(ef3ac4d2) SHA1(3cbae5282f6c5adb75c7bdc76f93350671389155) )
	j6firbl_sound
ROM_END

ROM_START( j6firblc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fire-2p1.bin", 0x000000, 0x020000, CRC(4a7a1f20) SHA1(dfdf29eb821548e17ce43c1c3089c23ff7f9ce5c) )
	ROM_LOAD16_BYTE( "fire-2_2.bin", 0x000001, 0x020000, CRC(ef3ac4d2) SHA1(3cbae5282f6c5adb75c7bdc76f93350671389155) )
	j6firbl_sound
ROM_END

ROM_START( j6firbld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fire-2w1.bin", 0x000000, 0x020000, CRC(c3893a90) SHA1(dca996d03c7fecd358b9a78352c0cf672b36b44a) )
	ROM_LOAD16_BYTE( "fire-2_2.bin", 0x000001, 0x020000, CRC(ef3ac4d2) SHA1(3cbae5282f6c5adb75c7bdc76f93350671389155) )
	j6firbl_sound
ROM_END


#define j6fireck_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "fccs1.bin", 0x000000, 0x080000, CRC(0421526d) SHA1(9dad850c208cb9f4a3a4c62e05a18217466d227e) )
ROM_START( j6fireck )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ficr-11.bin", 0x000000, 0x020000, CRC(655efa46) SHA1(f861bb97cd029353027379ab1a049218c3c987f5) )
	ROM_LOAD16_BYTE( "ficr-12.bin", 0x000001, 0x020000, CRC(b6f39b01) SHA1(67a80eb40923a282760ccb52c1265eff1c6623b2) )
	j6fireck_sound
ROM_END

ROM_START( j6firecka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3cbf", 0x000000, 0x020000, CRC(ba64f80f) SHA1(ae106127a2c085c7bc9758d0a03af2d94a625b42) )
	ROM_LOAD16_BYTE( "04a9", 0x000001, 0x020000, CRC(6401b1ac) SHA1(10f57318bb8cd4b6dc1e12538de697c77c4ce71c) )
	j6fireck_sound
ROM_END

ROM_START( j6fireckb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3cde", 0x000000, 0x020000, CRC(c7e48f46) SHA1(b81367724fb90834fc2431828ee9758dd19189d2) )
	ROM_LOAD16_BYTE( "04a9", 0x000001, 0x020000, CRC(6401b1ac) SHA1(10f57318bb8cd4b6dc1e12538de697c77c4ce71c) )
	j6fireck_sound
ROM_END

ROM_START( j6fireckc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3cf6", 0x000000, 0x020000, CRC(3208d492) SHA1(0d1947f80d2c8cc022229eff9120c689b48af4eb) )
	ROM_LOAD16_BYTE( "04a9", 0x000001, 0x020000, CRC(6401b1ac) SHA1(10f57318bb8cd4b6dc1e12538de697c77c4ce71c) )
	j6fireck_sound
ROM_END

ROM_START( j6fireckd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3cfe", 0x000000, 0x020000, CRC(033e6c56) SHA1(ec6f71eca170f7b7d367c476b0c742e1de6a597a) )
	ROM_LOAD16_BYTE( "04a9", 0x000001, 0x020000, CRC(6401b1ac) SHA1(10f57318bb8cd4b6dc1e12538de697c77c4ce71c) )
	j6fireck_sound
ROM_END

ROM_START( j6firecke )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "3cff", 0x000000, 0x020000, CRC(e8a0386e) SHA1(2127c9e2419f0a92803b670a2c1962c2e23122fa) )
	ROM_LOAD16_BYTE( "04a9", 0x000001, 0x020000, CRC(6401b1ac) SHA1(10f57318bb8cd4b6dc1e12538de697c77c4ce71c) )
	j6fireck_sound
ROM_END


#define j6firclb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "firecracker_club_sound.bin", 0x000000, 0x080000, CRC(0421526d) SHA1(9dad850c208cb9f4a3a4c62e05a18217466d227e) )
ROM_START( j6firclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "35ff", 0x000000, 0x020000, CRC(b0655026) SHA1(b0e556bbd1450035dd0a373eaf01a09d9cf90c60) ) // aka firecracker_club.p1
	ROM_LOAD16_BYTE( "13e6", 0x000001, 0x020000, CRC(2838d65f) SHA1(cfbb1cf5d9ee7c2b5ef1ace8e29436244c762e67) ) // aka firecracker_club.p2
	j6firclb_sound
ROM_END

ROM_START( j6firclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "35df", 0x000000, 0x020000, CRC(74bfb336) SHA1(03fce8fa82eaaa9c12f99b94a19f01143fce6ae6) )
	ROM_LOAD16_BYTE( "13e6", 0x000001, 0x020000, CRC(2838d65f) SHA1(cfbb1cf5d9ee7c2b5ef1ace8e29436244c762e67) )
	j6firclb_sound
ROM_END

ROM_START( j6firclbb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "35f7", 0x000000, 0x020000, CRC(8153e8e2) SHA1(0eb27f81c18ea09f45bdecb6d5985b95108383dd) )
	ROM_LOAD16_BYTE( "13e6", 0x000001, 0x020000, CRC(2838d65f) SHA1(cfbb1cf5d9ee7c2b5ef1ace8e29436244c762e67) )
	j6firclb_sound
ROM_END

ROM_START( j6firclbc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "35fe", 0x000000, 0x020000, CRC(5bfb041e) SHA1(b1009eaee844ea75665754f4026080f1497b421f) )
	ROM_LOAD16_BYTE( "13e6", 0x000001, 0x020000, CRC(2838d65f) SHA1(cfbb1cf5d9ee7c2b5ef1ace8e29436244c762e67) )
	j6firclb_sound
ROM_END


#define j6fivalv_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "fivealivesnd.bin", 0x000000, 0x080000, CRC(4e4e94d9) SHA1(b8d1f241c4257436fd0e552494d2c9af1c8661dd) )

ROM_START( j6fivalv )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "facl-6s1.bin", 0x000000, 0x020000, CRC(c52940cf) SHA1(bdfb4719d265f429f58400169d48ecab18b89296) )
	ROM_LOAD16_BYTE( "facl-6s2.bin", 0x000001, 0x020000, CRC(028959e5) SHA1(b5c64e4ddafe33642708ab9e13a8092e989ba0d6) )
	j6fivalv_sound
ROM_END

ROM_START( j6fivalva )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "facl-6f1.bin", 0x000000, 0x020000, CRC(01f3a3df) SHA1(93ab023af070f2d3edce0a729803e70103b38747) )
	ROM_LOAD16_BYTE( "facl-6s2.bin", 0x000001, 0x020000, CRC(028959e5) SHA1(b5c64e4ddafe33642708ab9e13a8092e989ba0d6) )
	j6fivalv_sound
ROM_END

ROM_START( j6fivalvb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "facl-6p1.bin", 0x000000, 0x020000, CRC(2eb714f7) SHA1(4e50e746c5c538ccd111fd40d8cb5cf25433f2f2) )
	ROM_LOAD16_BYTE( "facl-6s2.bin", 0x000001, 0x020000, CRC(028959e5) SHA1(b5c64e4ddafe33642708ab9e13a8092e989ba0d6) )
	j6fivalv_sound
ROM_END


#define j6fiveln_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "c-snd.bin", 0x000000, 0x080000, CRC(0016ab04) SHA1(82d133f485b325b29db901f6254c80ca959abd3e) )
ROM_START( j6fiveln )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "39.bin", 0x000000, 0x020000, CRC(f960d6dd) SHA1(d69f868201e1cd7ccceb155f6c219aa81791e3a3) )
	ROM_LOAD16_BYTE( "40.bin", 0x000001, 0x020000, CRC(ee1163eb) SHA1(f0723b67343a1f0c4cc7c20d2177ef5c3e156aed) )
	j6fiveln_sound
ROM_END

ROM_START( j6fivelna )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "41.bin", 0x000000, 0x020000, CRC(12fe82e5) SHA1(7ff4e94c0207df73ac20a390500e5ca7bf035524) )
	ROM_LOAD16_BYTE( "40.bin", 0x000001, 0x020000, CRC(ee1163eb) SHA1(f0723b67343a1f0c4cc7c20d2177ef5c3e156aed) )
	j6fiveln_sound
ROM_END

ROM_START( j6fivelnb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "42.bin", 0x000000, 0x020000, CRC(aba416bc) SHA1(31815dc414d94be7eaad5458a85f5a50c248ea99) )
	ROM_LOAD16_BYTE( "40.bin", 0x000001, 0x020000, CRC(ee1163eb) SHA1(f0723b67343a1f0c4cc7c20d2177ef5c3e156aed) )
	j6fiveln_sound
ROM_END

ROM_START( j6fivelnc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "43.bin", 0x000000, 0x020000, CRC(9b0da755) SHA1(a0e1e8da3333f3361a27505b7ca251a2c586251a) )
	ROM_LOAD16_BYTE( "40.bin", 0x000001, 0x020000, CRC(ee1163eb) SHA1(f0723b67343a1f0c4cc7c20d2177ef5c3e156aed) )
	j6fiveln_sound
ROM_END



#define j6frc10_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "force10snd.bin", 0x000000, 0x080000, CRC(e7e587c9) SHA1(fde7a7761253dc4133340b766d220873731c11c7) ) /* aka fo10-snd.bin */

ROM_START( j6frc10 )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9445.bin", 0x000000, 0x020000, CRC(d9d99afc) SHA1(3d2072ee7596f5d9dec8fc77af5963266afc2a75) ) // aka force1015-p1.bin
	ROM_LOAD16_BYTE( "9446.bin", 0x000001, 0x020000, CRC(50d87000) SHA1(02889506e8a009c85c73608db3196f88409007a1) ) // aka force1015-p2.bin
	j6frc10_sound
ROM_END

ROM_START( j6frc10a )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9447.bin", 0x000000, 0x020000, CRC(3247cec4) SHA1(3ec8619b4c1a75987b79f64263d9e17b6738a0fc) )
	ROM_LOAD16_BYTE( "9446.bin", 0x000001, 0x020000, CRC(50d87000) SHA1(02889506e8a009c85c73608db3196f88409007a1) )
	j6frc10_sound
ROM_END

ROM_START( j6frc10b )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9448.bin", 0x000000, 0x020000, CRC(8b1d5a9d) SHA1(8978660d6804a3886a0c08ce6bce2128491babbc) )
	ROM_LOAD16_BYTE( "9446.bin", 0x000001, 0x020000, CRC(50d87000) SHA1(02889506e8a009c85c73608db3196f88409007a1) )
	j6frc10_sound
ROM_END

ROM_START( j6frc10c )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9449.bin", 0x000000, 0x020000, CRC(bbb4eb74) SHA1(1b8f2290d332c273dd1f230cce62d96e890bc6bb) )
	ROM_LOAD16_BYTE( "9446.bin", 0x000001, 0x020000, CRC(50d87000) SHA1(02889506e8a009c85c73608db3196f88409007a1) )
	j6frc10_sound
ROM_END

ROM_START( j6frc10d )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "f1015p1", 0x000000, 0x020000, CRC(404fd4c9) SHA1(fa74aaa5fe554d223592fbf3e6f59999bbe04ec8) )
	ROM_LOAD16_BYTE( "f1015p2", 0x000001, 0x020000, CRC(36508a26) SHA1(e31a3e2e5e1048e087ef112cd9ef949a4e60228b) )
	j6frc10_sound
ROM_END



ROM_START( j6framft )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "frame.p1.bin", 0x000001, 0x020000, CRC(0bfaa4ff) SHA1(5080b3ce9692fe10b7e9e3fd75390513bbb60f99) )
	ROM_LOAD16_BYTE( "frame.p2.bin", 0x000000, 0x020000, CRC(a227fe30) SHA1(4b6034ea46c5c482e8b631af77ff6aac381eb941) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "frame_snd.bin", 0x000000, 0x080000, CRC(8053766a) SHA1(429e1476c7854a8a4d73aed0ec5a5efb31e6da4e) )
ROM_END


ROM_START( j6frtmch )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fruitmachine.p1", 0x000000, 0x020000, CRC(343f7fbc) SHA1(2d6594556cf4defb107d3a74c54ad9e1d2df63c6) )
	ROM_LOAD16_BYTE( "fruitmachine.p2", 0x000001, 0x020000, CRC(34c71b1f) SHA1(afc15df46fbcd845b963adcfe76764eba58da1e3) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "fruitmachinesnd.bin", 0x000000, 0x080000, CRC(24df2399) SHA1(8ed92b710d866b852143989f2f8d84da90cd1a63) )
ROM_END


#define j6frtpot_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "fruitpotssnd.bin", 0x000000, 0x080000, CRC(1aacc429) SHA1(7ee38a34087a05d06fbfff78b57bf794c4f25d0c) )

ROM_START( j6frtpot )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF ) // not overdumps? legit larger game?
	ROM_LOAD16_BYTE( "fruitpots.p1", 0x000000, 0x080000, CRC(c8c5ebf7) SHA1(39040b08d6b67723388c4d90433f0965637590fb) )
	ROM_LOAD16_BYTE( "fruitpots.p2", 0x000001, 0x080000, CRC(5922976b) SHA1(a11ced9f363a085c5e77e5a85d5bbc785b6600c0) )
	j6frtpot_sound
ROM_END

ROM_START( j6frtpota )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF ) // not overdumps? legit larger game?
	ROM_LOAD16_BYTE( "60000200.evn", 0x000000, 0x080000, CRC(1cd3d39f) SHA1(9cd873c16c5698e7fe9e07bbd05481e5edb8a0e0) )
	ROM_LOAD16_BYTE( "60000200.odd", 0x000001, 0x080000, CRC(7e338bc0) SHA1(b970815d7cb951a349a4ddafcb834fa3c793a2d8) )
	j6frtpot_sound
ROM_END

ROM_START( j6frtpotb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF ) // not overdumps? legit larger game?
	ROM_LOAD16_BYTE( "60001201.evn", 0x000000, 0x080000, CRC(42bd9e8e) SHA1(11e9e1fa94651ed566075244a3718966eff9c86c) )
	ROM_LOAD16_BYTE( "60001201.odd", 0x000001, 0x080000, CRC(7cfc531a) SHA1(583b58793184001d0e66b7a7922ecc26e5f78b79) )
	j6frtpot_sound
ROM_END

ROM_START( j6frtpotc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF ) // not overdumps? legit larger game?
	ROM_LOAD16_BYTE( "60001202.evn", 0x000000, 0x080000, CRC(e586058b) SHA1(4322dc1f21a17fcec8dbface7d0a545dc95191ec) )
	ROM_LOAD16_BYTE( "60001202.odd", 0x000001, 0x080000, CRC(2d70ab1d) SHA1(e1c9ad7b498ea2de8bc82631ab5468bad7ff4225) )
	j6frtpot_sound
ROM_END


#define j6gforce_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "gfor-snd.bin", 0x000000, 0x080000, CRC(4b710c8a) SHA1(af93c795d4c46cb95d92c48ac60a48db7f6724ac) )

ROM_START( j6gforce )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gfor-3s1.bin", 0x000000, 0x020000, CRC(a289af04) SHA1(d8a5de1ea8dddaf276693b7f1858442211ca0d79) )
	ROM_LOAD16_BYTE( "gfor-3s2.bin", 0x000001, 0x020000, CRC(fc02d84f) SHA1(80f1066896a2e24c202b9754c04b542b909f6658) )
	j6gforce_sound
ROM_END

ROM_START( j6gforcea )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gfor-3a1.bin", 0x000000, 0x020000, CRC(f04d6f65) SHA1(72dcd029e04afeed849fe271bafb731c8735420f) )
	ROM_LOAD16_BYTE( "gfor-3s2.bin", 0x000001, 0x020000, CRC(fc02d84f) SHA1(80f1066896a2e24c202b9754c04b542b909f6658) )
	j6gforce_sound
ROM_END

ROM_START( j6gforceb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gfor-3p1.bin", 0x000000, 0x020000, CRC(4917fb3c) SHA1(d2badd3cc06cadff62a3e1e43e44ff5fe084c00b) )
	ROM_LOAD16_BYTE( "gfor-3s2.bin", 0x000001, 0x020000, CRC(fc02d84f) SHA1(80f1066896a2e24c202b9754c04b542b909f6658) )
	j6gforce_sound
ROM_END

ROM_START( j6gforcec )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gfor-3wp.bin", 0x000000, 0x020000, CRC(2b7a8ab4) SHA1(ffa0cb80194add811ccddb80fffcf8fa91491ec0) )
	ROM_LOAD16_BYTE( "gfor-3s2.bin", 0x000001, 0x020000, CRC(fc02d84f) SHA1(80f1066896a2e24c202b9754c04b542b909f6658) )
	j6gforce_sound
ROM_END

ROM_START( j6gforced )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "g_force.p1",   0x000000, 0x020000, CRC(724afeba) SHA1(b7eca2138b7c04031fa3b6a35a91180a7b487920) )
	ROM_LOAD16_BYTE( "g_force.p2",   0x000001, 0x020000, CRC(bc5a491e) SHA1(a8df6f26396c6d54b9de82717231f56342373516) )
	j6gforce_sound
ROM_END

ROM_START( j6gidogh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gidough.p1", 0x000000, 0x020000, CRC(92b2ad84) SHA1(710630c05dafeaaa65d913348ac545d1d74e900e) )
	ROM_LOAD16_BYTE( "gidough.p2", 0x000001, 0x020000, CRC(a997c758) SHA1(0ddd76d78e5efe4ae1e044eedf2d3710bde99224) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "gidoughsnd.bin", 0x000000, 0x080000, CRC(69339dd1) SHA1(e69445471ff5f102d1a234f0859b01b3f7a82498) )
ROM_END


#define j6guab_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "guabs.hex", 0x000000, 0x080000, CRC(fc041c87) SHA1(bd2606e3a67e13ce937b8cb4d5fcda9fa13842a1) )

ROM_START( j6guab )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8574.bin", 0x000000, 0x020000, CRC(a58dc7e1) SHA1(b853217ccbae59e9485931464dd808d2684c331a) )
	ROM_LOAD16_BYTE( "8575.bin", 0x000001, 0x020000, CRC(dd8ff2cb) SHA1(400693667f6b459421a9589546b113286c58508b) )
	j6guab_sound
ROM_END

ROM_START( j6guaba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8576.bin", 0x000000, 0x020000, CRC(4e1393d9) SHA1(8d88bcb8ff267de916b279ce5347963d88081cf6) )
	ROM_LOAD16_BYTE( "8575.bin", 0x000001, 0x020000, CRC(dd8ff2cb) SHA1(400693667f6b459421a9589546b113286c58508b) )
	j6guab_sound
ROM_END

ROM_START( j6guabb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8577.bin", 0x000000, 0x020000, CRC(c7e0b669) SHA1(2e2baa1c7b3d91a2af63eb97999b1a15a65a9b0a) )
	ROM_LOAD16_BYTE( "8575.bin", 0x000001, 0x020000, CRC(dd8ff2cb) SHA1(400693667f6b459421a9589546b113286c58508b) )
	j6guab_sound
ROM_END

ROM_START( j6guabc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9305.bin", 0x000000, 0x020000, CRC(cfaab78e) SHA1(86161fe76d7d462dc3d6b8e466850bce14ba21ed) )
	ROM_LOAD16_BYTE( "9306.bin", 0x000001, 0x020000, CRC(1273a76c) SHA1(f2fcb3cb2780c8dd530727cffda32700fa046060) )
	j6guab_sound
ROM_END

ROM_START( j6guabd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9307.bin", 0x000000, 0x020000, CRC(2434e3b6) SHA1(d09bb8dfd51fd2b52a277e92009623587bd14399) )
	ROM_LOAD16_BYTE( "9306.bin", 0x000001, 0x020000, CRC(1273a76c) SHA1(f2fcb3cb2780c8dd530727cffda32700fa046060) )
	j6guab_sound
ROM_END

ROM_START( j6guabe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9308.bin", 0x000000, 0x020000, CRC(9d6e77ef) SHA1(b5f6a9acb31259d2f57d1bf3d019a92fd52c424a) )
	ROM_LOAD16_BYTE( "9306.bin", 0x000001, 0x020000, CRC(1273a76c) SHA1(f2fcb3cb2780c8dd530727cffda32700fa046060) )
	j6guab_sound
ROM_END

ROM_START( j6guabf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9309.bin", 0x000000, 0x020000, CRC(adc7c606) SHA1(4ee51b8ec543ab8efe62ed95b8a30248c24467b0) )
	ROM_LOAD16_BYTE( "9306.bin", 0x000001, 0x020000, CRC(1273a76c) SHA1(f2fcb3cb2780c8dd530727cffda32700fa046060) )
	j6guab_sound
ROM_END


#define j6guabcl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "guabs.hex", 0x000000, 0x080000, CRC(fc041c87) SHA1(bd2606e3a67e13ce937b8cb4d5fcda9fa13842a1) )

ROM_START( j6guabcl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8841.bin", 0x000000, 0x020000, CRC(8483bf47) SHA1(08726c55a1064ecb392e904b748f74032e77a3c9) )
	ROM_LOAD16_BYTE( "8842.bin", 0x000001, 0x020000, CRC(f203d5ec) SHA1(5748e5f482f771de3348217170e7fa0d4986048e) )
	j6guabcl_sound
ROM_END

ROM_START( j6guabcla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8843.bin", 0x000000, 0x020000, CRC(6f1deb7f) SHA1(53f00ac5552838448e0329928e70db3c98fdd65f) )
	ROM_LOAD16_BYTE( "8842.bin", 0x000001, 0x020000, CRC(f203d5ec) SHA1(5748e5f482f771de3348217170e7fa0d4986048e) )
	j6guabcl_sound
ROM_END


ROM_START( j6reelb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fg81.bin", 0x00000, 0x020000, CRC(570cd8b3) SHA1(91563b30d2ff229a000dfb3299a9cf343517fc72) )
	ROM_LOAD16_BYTE(  "fg82.bin", 0x00001, 0x020000, CRC(952a7743) SHA1(908e03279f0b98921b417e4c85117e0ef7f5c8b3) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "j6reelb.upd", 0x000000, 0x080000, NO_DUMP )
ROM_END

ROM_START( j6reelba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fg81p.bin", 0x0000, 0x020000, CRC(bc928c8b) SHA1(6411994eebdfaac494725f64446cb711eb54c2b7) )
	ROM_LOAD16_BYTE(  "fg82.bin", 0x00001, 0x020000, CRC(952a7743) SHA1(908e03279f0b98921b417e4c85117e0ef7f5c8b3) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "j6reelb.upd", 0x000000, 0x080000, NO_DUMP )
ROM_END



#define j6gldclb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "gladiatorsnd.bin", 0x000000, 0x080000, CRC(13bd21c7) SHA1(3e0e087fdf8566ca6803f8f9f75597e19433fd0b) )

ROM_START( j6gldclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clgl-as1.bin", 0x000000, 0x020000, CRC(16b8560f) SHA1(68d3577bc14a60ca2c19091c05d1a65b7eae6747) )
	ROM_LOAD16_BYTE( "clgl-as2.bin", 0x000001, 0x020000, CRC(dbcf8013) SHA1(39247fcf209b1007cde1e161d72165eeb239a23c) )
	j6gldclb_sound
ROM_END

ROM_START( j6gldclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clgl-af1.bin", 0x000000, 0x020000, CRC(d262b51f) SHA1(072638e672e9ace6bac08c23073c8400d7d2315a) )
	ROM_LOAD16_BYTE( "clgl-as2.bin", 0x000001, 0x020000, CRC(dbcf8013) SHA1(39247fcf209b1007cde1e161d72165eeb239a23c) )
	j6gldclb_sound
ROM_END

ROM_START( j6gldclbb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clgl-ap1.bin", 0x000000, 0x020000, CRC(fd260237) SHA1(08873e61f75227b73ea030f6a63a7f5917552f7b) )
	ROM_LOAD16_BYTE( "clgl-as2.bin", 0x000001, 0x020000, CRC(dbcf8013) SHA1(39247fcf209b1007cde1e161d72165eeb239a23c) )
	j6gldclb_sound
ROM_END


#define j6gogold_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "gfgoldsnd.bin", 0x000000, 0x080000, CRC(1ccc9b9b) SHA1(d6c7d4285b569c8ed77f732d6e42e6b763d200d4) )
ROM_START( j6gogold )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20702.bin", 0x000000, 0x020000, CRC(c274df22) SHA1(f623bb8ba2afcc3ad7c58a4cf56ea8d8f9d1308a) )
	ROM_LOAD16_BYTE( "20703.bin", 0x000001, 0x020000, CRC(aa2a1e67) SHA1(86fc1962a4de05f3eca8ca0b02d04db005e8a174) )
	j6gogold_sound
ROM_END

ROM_START( j6gogolda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20704.bin", 0x000000, 0x020000, CRC(29ea8b1a) SHA1(da066336de791891a35201e51f92e1cd4190f488) )
	ROM_LOAD16_BYTE( "20703.bin", 0x000001, 0x020000, CRC(aa2a1e67) SHA1(86fc1962a4de05f3eca8ca0b02d04db005e8a174) )
	j6gogold_sound
ROM_END

ROM_START( j6gogoldb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20705.bin", 0x000000, 0x020000, CRC(90b01f43) SHA1(ff28bbee32f5d2192a6ea391dd4f5103c1f12296) )
	ROM_LOAD16_BYTE( "20703.bin", 0x000001, 0x020000, CRC(aa2a1e67) SHA1(86fc1962a4de05f3eca8ca0b02d04db005e8a174) )
	j6gogold_sound
ROM_END

ROM_START( j6gogoldc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20706.bin", 0x000000, 0x020000, CRC(a019aeaa) SHA1(e7d83c0b4b232687ed6491620e0b22cc93e60265) )
	ROM_LOAD16_BYTE( "20703.bin", 0x000001, 0x020000, CRC(aa2a1e67) SHA1(86fc1962a4de05f3eca8ca0b02d04db005e8a174) )
	j6gogold_sound
ROM_END




ROM_START( j6gldmin )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gm1_1.p1", 0x000000, 0x020000, CRC(a6a135bb) SHA1(2a143fd0f4b6227b2a528a4c65865ecd781706dd) )
	ROM_LOAD16_BYTE( "gm1_1.p2", 0x000001, 0x020000, CRC(6027f0f7) SHA1(f68a5f33c3d2f04dc892a0be115594e5aa577682) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "gmsnd.bin", 0x000000, 0x080000, CRC(23913559) SHA1(3c71eea6f847a6eb16f76a29555c9fde5790929a) )
ROM_END




#define j6golddm_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "gdsnd.bin", 0x000000, 0x04a018, CRC(796e1b35) SHA1(e9c8e5a350823275c9ba9238781872ea359d5049) )

ROM_START( j6golddm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gd30cz03_1.b8", 0x000000, 0x020000, CRC(bf901be5) SHA1(c508706f77ff23086d5507823fa29784f9c2d83c) )
	ROM_LOAD16_BYTE( "gd30cz03_2.b8", 0x000001, 0x020000, CRC(93bd2009) SHA1(a0e2ffb9dfad123e884507e3df26aa3d457788ff) )
	j6golddm_sound
ROM_END

ROM_START( j6golddma )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gd75cz03_1.b8", 0x000000, 0x020000, CRC(b3ddb5d4) SHA1(dbdf8f9c558355a20fce83470c20768358b7c819) )
	ROM_LOAD16_BYTE( "gd30cz03_2.b8", 0x000001, 0x020000, CRC(93bd2009) SHA1(a0e2ffb9dfad123e884507e3df26aa3d457788ff) )
	j6golddm_sound
ROM_END

#define j6goldgl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "goldengoalsnd.bin", 0x000000, 0x080000, CRC(3af9ccdb) SHA1(4a911a48816bc69743ba1ba18fdd913041636ae1) )

ROM_START( j6goldgl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "goal-11.bin", 0x000000, 0x020000, CRC(124870c1) SHA1(7f42ae51f342beaf0c53f46df437ea81772f1005) )
	ROM_LOAD16_BYTE( "goal-12.bin", 0x000001, 0x020000, CRC(5e292400) SHA1(74d6a480881b4fb5deac921517e8c07586ade4f3) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldgla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo-11.bin", 0x000000, 0x020000, CRC(f73cd3e9) SHA1(4068eb63f696910a0d04da3bad727f0d2225fb47) )
	ROM_LOAD16_BYTE( "gogo-12.bin", 0x000001, 0x020000, CRC(7662ec60) SHA1(1ea6c11ab0c58b92211cb7feded258ff33c4e592) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldglb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo-1a1.bin", 0x000000, 0x020000, CRC(a5f81388) SHA1(dd832fcbc63e967213bf133a50337a877f18f413) )
	ROM_LOAD16_BYTE( "gogo-12.bin", 0x000001, 0x020000, CRC(7662ec60) SHA1(1ea6c11ab0c58b92211cb7feded258ff33c4e592) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldglc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo-1p1.bin", 0x000000, 0x020000, CRC(1ca287d1) SHA1(eda47e49a1f832c3cff096d3a1fe904d0bf2a2b3) )
	ROM_LOAD16_BYTE( "gogo-12.bin", 0x000001, 0x020000, CRC(7662ec60) SHA1(1ea6c11ab0c58b92211cb7feded258ff33c4e592) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldgld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo-31.bin", 0x000000, 0x020000, CRC(8d4c0b6b) SHA1(ce9bbb28e0c3e92693dbf1fbd2562bd697d54431) )
	ROM_LOAD16_BYTE( "gogo-32.bin", 0x000001, 0x020000, CRC(9c36cb43) SHA1(bbc143d3727130e123fffd295422fdfc76c85d12) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldgle )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo-3a1.bin", 0x000000, 0x020000, CRC(df88cb0a) SHA1(871e1ac825851f0d61c2dd5a9f2f2c768990032f) )
	ROM_LOAD16_BYTE( "gogo-32.bin", 0x000001, 0x020000, CRC(9c36cb43) SHA1(bbc143d3727130e123fffd295422fdfc76c85d12) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldglf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo-3l1.bin", 0x000000, 0x020000, CRC(bc7ab3af) SHA1(fd539a5d9c9e1e3233782c278c465e6789414563) )
	ROM_LOAD16_BYTE( "gogo-32.bin", 0x000001, 0x020000, CRC(9c36cb43) SHA1(bbc143d3727130e123fffd295422fdfc76c85d12) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldglg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo-3p1.bin", 0x000000, 0x020000, CRC(66d25f53) SHA1(9bb788fb1330ff3dc6c9abd166adcaafd67ee56b) )
	ROM_LOAD16_BYTE( "gogo-32.bin", 0x000001, 0x020000, CRC(9c36cb43) SHA1(bbc143d3727130e123fffd295422fdfc76c85d12) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldglh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "gogo3lp1.bin", 0x000000, 0x020000, CRC(57e4e797) SHA1(3fac191bc880596427ce563f045227f181e3c2df) )
	ROM_LOAD16_BYTE( "gogo-32.bin", 0x000001, 0x020000, CRC(9c36cb43) SHA1(bbc143d3727130e123fffd295422fdfc76c85d12) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldgli )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "2412", 0x000000, 0x020000, CRC(54a6ab53) SHA1(24ae5a00fea82a910494fedb80546fe7d0b65313) )
	ROM_LOAD16_BYTE( "8832", 0x000001, 0x020000, CRC(24eb5d3a) SHA1(3a26cf8f80c7843495fbdc93a4d31876206d0004) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldglj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "244a", 0x000000, 0x020000, CRC(3754d3f6) SHA1(a2106953e1b89b18f78052fcdb34bc8ac656f050) )
	ROM_LOAD16_BYTE( "8832", 0x000001, 0x020000, CRC(24eb5d3a) SHA1(3a26cf8f80c7843495fbdc93a4d31876206d0004) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldglk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "2451", 0x000000, 0x020000, CRC(edfc3f0a) SHA1(2acfd13b3d77c682f204797114389e06296dddd7) )
	ROM_LOAD16_BYTE( "8832", 0x000001, 0x020000, CRC(24eb5d3a) SHA1(3a26cf8f80c7843495fbdc93a4d31876206d0004) )
	j6goldgl_sound
ROM_END

ROM_START( j6goldgll )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "2452", 0x000000, 0x020000, CRC(06626b32) SHA1(2245bc58df15a455dfc47e66afc82e4230f0088a) )
	ROM_LOAD16_BYTE( "8832", 0x000001, 0x020000, CRC(24eb5d3a) SHA1(3a26cf8f80c7843495fbdc93a4d31876206d0004) )
	j6goldgl_sound
ROM_END



#define j6hapyhr_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "happyhoursnd.bin", 0x000000, 0x080000, CRC(ef80bbfd) SHA1(66dc0bd35054a506dc75972ac59f9ca03d886e1b) )

ROM_START( j6hapyhr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20426.bin", 0x000000, 0x020000, CRC(58f6ceb5) SHA1(5ca6861d1532ede848f672fe08152dcd0f65be95) )
	ROM_LOAD16_BYTE( "20427.bin", 0x000001, 0x020000, CRC(3de94b07) SHA1(ce1a712845ccc5fa9ef92b3d07f8872afeec88f8) )
	j6hapyhr_sound
ROM_END

ROM_START( j6hapyhra )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20428.bin", 0x000000, 0x020000, CRC(b3689a8d) SHA1(4f690ec96f1b5e0ed30023016de767c132356430) )
	ROM_LOAD16_BYTE( "20427.bin", 0x000001, 0x020000, CRC(3de94b07) SHA1(ce1a712845ccc5fa9ef92b3d07f8872afeec88f8) )
	j6hapyhr_sound
ROM_END

ROM_START( j6hapyhrb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20429.bin", 0x000000, 0x020000, CRC(0a320ed4) SHA1(5bf54339717694febbf6749c39985fc7ff4c194f) ) // aka happyhour.p1
	ROM_LOAD16_BYTE( "20427.bin", 0x000001, 0x020000, CRC(3de94b07) SHA1(ce1a712845ccc5fa9ef92b3d07f8872afeec88f8) ) // aka happyhour.p2
	j6hapyhr_sound
ROM_END



ROM_START( j6hifly )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiflyer.p1", 0x000000, 0x020000, CRC(5309d1ec) SHA1(49df13b33dd67a8ed63cdd9b83d6c58b34f29d67) )
	ROM_LOAD16_BYTE( "hiflyer.p2", 0x000001, 0x020000, CRC(6cae4a62) SHA1(acf196220dca131bc274d91164204aeeec8fc08c) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "hiflyersnd.bin", 0x000000, 0x080000, CRC(a0f1b9d2) SHA1(738e9a6ac1d8af1e1610d96195a86ce34fe53e33) )
ROM_END

#define j6impact_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "hiim-snd.bin", 0x000000, 0x080000, CRC(3f54a54c) SHA1(fb3b2561f10391f01ee97e4501e8492fcfe4fd2b) )

ROM_START( j6impact )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiim-2s1.bin", 0x000000, 0x020000, CRC(9cfa16b2) SHA1(520d38ec96652914c6506c55fd59c01fcb8a67c4) )
	ROM_LOAD16_BYTE( "hiim-2s2.bin", 0x000001, 0x020000, CRC(8a087a6d) SHA1(38463d0a474f3a2d137b7e2a825d8e65d9d043c0) )
	j6impact_sound
ROM_END

ROM_START( j6impacta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiim-2a1.bin", 0x000000, 0x020000, CRC(ce3ed6d3) SHA1(a059a2f279df4f8432a9e7fa780ed174d63d2ac5) )
	ROM_LOAD16_BYTE( "hiim-2s2.bin", 0x000001, 0x020000, CRC(8a087a6d) SHA1(38463d0a474f3a2d137b7e2a825d8e65d9d043c0) )
	j6impact_sound
ROM_END

ROM_START( j6impactb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiim-2p1.bin", 0x000000, 0x020000, CRC(7764428a) SHA1(4f83a0d3ce7c484617122191a66e90432aa39e4e) )
	ROM_LOAD16_BYTE( "hiim-2s2.bin", 0x000001, 0x020000, CRC(8a087a6d) SHA1(38463d0a474f3a2d137b7e2a825d8e65d9d043c0) )
	j6impact_sound
ROM_END

ROM_START( j6impactc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hi_imp.p1",    0x000000, 0x020000, CRC(d742d6c9) SHA1(cf4ccc22a2cabfe06339ab079f7e5b9bb6297e8b) )
	ROM_LOAD16_BYTE( "hi_imp.p2",    0x000001, 0x020000, CRC(9c3de3c5) SHA1(428d101146a99ae713251ccf070049c0985b577b) )
	j6impact_sound
ROM_END

#define j6hilosv_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "hi_lo_silver_snd.bin", 0x000000, 0x080000, CRC(fd88e3a6) SHA1(07c2fec617faea189ceddc46ec477fb09c0ec4a9) )

ROM_START( j6hilosv )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9287.bin", 0x000000, 0x020000, CRC(9ea4dec0) SHA1(14a9239fb3a94f8f3c4a46c5f1d189a3f4a54868) )
	ROM_LOAD16_BYTE( "9288.bin", 0x000001, 0x020000, CRC(97184aba) SHA1(5b95e138998e78a8bf7f12186fd9f1849d2efa7b) )
	j6hilosv_sound
ROM_END

ROM_START( j6hilosva )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9289.bin", 0x000000, 0x020000, CRC(753a8af8) SHA1(258c390235a45d9b08f13e975807a379f558453e) )
	ROM_LOAD16_BYTE( "9288.bin", 0x000001, 0x020000, CRC(97184aba) SHA1(5b95e138998e78a8bf7f12186fd9f1849d2efa7b) )
	j6hilosv_sound
ROM_END

ROM_START( j6hilosvb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9290.bin", 0x000000, 0x020000, CRC(cc601ea1) SHA1(6a5637dd37c83355817294090596ed5ed678509e) )
	ROM_LOAD16_BYTE( "9288.bin", 0x000001, 0x020000, CRC(97184aba) SHA1(5b95e138998e78a8bf7f12186fd9f1849d2efa7b) )
	j6hilosv_sound
ROM_END

ROM_START( j6hilosvc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9291.bin", 0x000000, 0x020000, CRC(fcc9af48) SHA1(3f1cd018b48c7fe55bb16981d00b69892dadd241) )
	ROM_LOAD16_BYTE( "9288.bin", 0x000001, 0x020000, CRC(97184aba) SHA1(5b95e138998e78a8bf7f12186fd9f1849d2efa7b) )
	j6hilosv_sound
ROM_END

// from a 'crystal' set
ROM_START( j6hilosvd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20575.bin", 0x000000, 0x020000, CRC(97958436) SHA1(8d6afa9431a217eb022223ba5875321b3e6a9546) )
	ROM_LOAD16_BYTE( "20576.bin", 0x000001, 0x020000, NO_DUMP ) // assumed name
	j6hilosv_sound
ROM_END

ROM_START( j6hilosve )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20577.bin", 0x000000, 0x020000, CRC(c5514457) SHA1(a6cfcb2b22f24fa994f9913c6efbb0495c77e6f8) )
	ROM_LOAD16_BYTE( "20576.bin", 0x000001, 0x020000, NO_DUMP ) // assumed name
	j6hilosv_sound
ROM_END


#define j6hirol_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "hiro-snd.bin", 0x000000, 0x080000, CRC(5843c195) SHA1(0665e913e4c1a919aa5331cce7a467c841722388) )

ROM_START( j6hirol )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiro-1s1.bin", 0x000000, 0x020000, CRC(98e426ce) SHA1(16140aad2efa8b19eedb411909dccdb5ca5561cf) )
	ROM_LOAD16_BYTE( "hiro-1s2.bin", 0x000001, 0x020000, CRC(6b903488) SHA1(c62e3f5446c62d45926190d48fb2a34916d1f098) )
	j6hirol_sound
ROM_END

ROM_START( j6hirola )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiro-1a1.bin", 0x000000, 0x020000, CRC(ca20e6af) SHA1(a6b2e94cf13b2f5628426d4b26f44c69b45896bc) )
	ROM_LOAD16_BYTE( "hiro-1s2.bin", 0x000001, 0x020000, CRC(6b903488) SHA1(c62e3f5446c62d45926190d48fb2a34916d1f098) )
	j6hirol_sound
ROM_END

ROM_START( j6hirolb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiro-1p1.bin", 0x000000, 0x020000, CRC(737a72f6) SHA1(d2658e91b283ec8fe860d4dc9a130afcaac525bb) )
	ROM_LOAD16_BYTE( "hiro-1s2.bin", 0x000001, 0x020000, CRC(6b903488) SHA1(c62e3f5446c62d45926190d48fb2a34916d1f098) )
	j6hirol_sound
ROM_END

ROM_START( j6hirolc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiro-1w1.bin", 0x000000, 0x020000, CRC(fa895746) SHA1(a5ad82d76c96004d578cc217a46b0eef78e867ac) )
	ROM_LOAD16_BYTE( "hiro-1s2.bin", 0x000001, 0x020000, CRC(6b903488) SHA1(c62e3f5446c62d45926190d48fb2a34916d1f098) )
	j6hirol_sound
ROM_END

ROM_START( j6hirold )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiro-1wp.bin", 0x000000, 0x020000, CRC(1117037e) SHA1(8488ef64d50d78a55857d24a0d25e4060e34c35e) )
	ROM_LOAD16_BYTE( "hiro-1s2.bin", 0x000001, 0x020000, CRC(6b903488) SHA1(c62e3f5446c62d45926190d48fb2a34916d1f098) )
	j6hirol_sound
ROM_END

#define j6hirlcl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "hrcl-snd.bin", 0x000000, 0x080000, CRC(5843c195) SHA1(0665e913e4c1a919aa5331cce7a467c841722388) )
ROM_START( j6hirlcl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hrcl-3_1.bin", 0x000000, 0x020000, CRC(680182bf) SHA1(54e1b6db179934a29453ff5c5664fd00352377ea) )
	ROM_LOAD16_BYTE( "hrcl-3_2.bin", 0x000001, 0x020000, CRC(c096ad69) SHA1(1a778f4a54bc98db32e5df20fb74318bea00f6d3) )
	j6hirlcl_sound
ROM_END

ROM_START( j6hirlcla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hrcl-3f1.bin", 0x000000, 0x020000, CRC(acdb61af) SHA1(80b372c584aefd0534ed0be2c5f0ac44341bde18) )
	ROM_LOAD16_BYTE( "hrcl-3_2.bin", 0x000001, 0x020000, CRC(c096ad69) SHA1(1a778f4a54bc98db32e5df20fb74318bea00f6d3) )
	j6hirlcl_sound
ROM_END

ROM_START( j6hirlclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hrcl-3n1.bin", 0x000000, 0x020000, CRC(0a6cf337) SHA1(812f87724ebd6f5373ea4914efce5ebd64a2cb35) )
	ROM_LOAD16_BYTE( "hrcl-3_2.bin", 0x000001, 0x020000, CRC(c096ad69) SHA1(1a778f4a54bc98db32e5df20fb74318bea00f6d3) )
	j6hirlcl_sound
ROM_END

ROM_START( j6hirlclc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hrcl-3p1.bin", 0x000000, 0x020000, CRC(839fd687) SHA1(c0259308321c8f1d8c1ba1dad333e223c06e9c4a) )
	ROM_LOAD16_BYTE( "hrcl-3_2.bin", 0x000001, 0x020000, CRC(c096ad69) SHA1(1a778f4a54bc98db32e5df20fb74318bea00f6d3) )
	j6hirlcl_sound
ROM_END

#define j6histk_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "histakessnd.bin", 0x000000, 0x080000, CRC(7bffa191) SHA1(e3a4a4eef878fb093240a3e145cf405d266bec74) )

ROM_START( j6histk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60000082.p1", 0x000000, 0x020000, CRC(37de3dd8) SHA1(3d5aaf9112ca79eeb72bcbd78cf5a89c26ecc9e1) )
	ROM_LOAD16_BYTE( "60000082.p2", 0x000001, 0x020000, CRC(91614eea) SHA1(7869d9f0326ef5b3b7841a4430e7f90dcb6dfb96) )
	j6histk_sound
ROM_END

ROM_START( j6histka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "histakes.p1", 0x000000, 0x020000, CRC(8a021897) SHA1(7b8bd6add7f3341a719a384680615abe8e19cbac) )
	ROM_LOAD16_BYTE( "histakes.p2", 0x000001, 0x020000, CRC(12467e9d) SHA1(d2f3caeaf63392f2b6ba157e054cb40c1a73c19d) )
	j6histk_sound
ROM_END

#define j6hiphop_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6hiphop )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiho-11.bin", 0x000000, 0x020000, CRC(163aa788) SHA1(a7047fa9b6273eb5749195914c098a524e0fb68a) )
	ROM_LOAD16_BYTE( "hiho-12.bin", 0x000001, 0x020000, CRC(8d8cb4b4) SHA1(6baea853b5e0bc349980d3ef7d93a3f4c146492a) )
	j6hiphop_sound
ROM_END

ROM_START( j6hiphopa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiho-1a1.bin", 0x000000, 0x020000, CRC(44fe67e9) SHA1(f8089203051ab3c54959bffd91064042b4ddcd55) )
	ROM_LOAD16_BYTE( "hiho-12.bin", 0x000001, 0x020000, CRC(8d8cb4b4) SHA1(6baea853b5e0bc349980d3ef7d93a3f4c146492a) )
	j6hiphop_sound
ROM_END

ROM_START( j6hiphopb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiho-1n1.bin", 0x000000, 0x020000, CRC(270c1f4c) SHA1(f73c96dc08f3eb6d8493476f1323d4df30e1dc8a) )
	ROM_LOAD16_BYTE( "hiho-12.bin", 0x000001, 0x020000, CRC(8d8cb4b4) SHA1(6baea853b5e0bc349980d3ef7d93a3f4c146492a) )
	j6hiphop_sound
ROM_END

ROM_START( j6hiphopc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiho-1p1.bin", 0x000000, 0x020000, CRC(fda4f3b0) SHA1(01702290b1c6ac25a221b24e2607aa68205284eb) )
	ROM_LOAD16_BYTE( "hiho-12.bin", 0x000001, 0x020000, CRC(8d8cb4b4) SHA1(6baea853b5e0bc349980d3ef7d93a3f4c146492a) )
	j6hiphop_sound
ROM_END

ROM_START( j6hiphopd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hiho1np1.bin", 0x000000, 0x020000, CRC(cc924b74) SHA1(2a82f30252a695f7bc196f6699b0871b938b7fdd) )
	ROM_LOAD16_BYTE( "hiho-12.bin", 0x000001, 0x020000, CRC(8d8cb4b4) SHA1(6baea853b5e0bc349980d3ef7d93a3f4c146492a) )
	j6hiphop_sound
ROM_END


#define j6hotsht_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "hosh-snd.bin", 0x000000, 0x080000, CRC(f5bcfe63) SHA1(4983cb4c2d69730d7f1984d648c2801b46b4ab70) )

ROM_START( j6hotsht )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-4_1.bin", 0x000000, 0x020000, CRC(833c49e9) SHA1(94552fc3c2c246412e4c62e5095c1fc3707fd73c) )
	ROM_LOAD16_BYTE( "hosh-4_2.bin", 0x000001, 0x020000, CRC(645c832d) SHA1(174dc5491d567efd37051ca10987e24c8ccea5e8) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-4a1.bin", 0x000000, 0x020000, CRC(d1f88988) SHA1(5063408d5586d0dedb535c47ce725ab7365c7eef) )
	ROM_LOAD16_BYTE( "hosh-4_2.bin", 0x000001, 0x020000, CRC(645c832d) SHA1(174dc5491d567efd37051ca10987e24c8ccea5e8) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-4p1.bin", 0x000000, 0x020000, CRC(68a21dd1) SHA1(68700e0c30e715795e604d1ff7606e03c4d93362) )
	ROM_LOAD16_BYTE( "hosh-4_2.bin", 0x000001, 0x020000, CRC(645c832d) SHA1(174dc5491d567efd37051ca10987e24c8ccea5e8) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-4w1.bin", 0x000000, 0x020000, CRC(e1513861) SHA1(8c945a6c52670754ddeece94c0ebdb046f238405) )
	ROM_LOAD16_BYTE( "hosh-4_2.bin", 0x000001, 0x020000, CRC(645c832d) SHA1(174dc5491d567efd37051ca10987e24c8ccea5e8) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-51.bin", 0x000000, 0x020000, CRC(d54e81eb) SHA1(2e67f0f3ee2fceeb56c93f6a5140395d341bf414) )
	ROM_LOAD16_BYTE( "hosh-52.bin", 0x000001, 0x020000, CRC(39b2798d) SHA1(9d66f99b4b3ffaa07018e4c70bb83903d89c0a50) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshte )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-5a1.bin", 0x000000, 0x020000, CRC(878a418a) SHA1(32d6a5c83d65132ca4f8646661f743b3e8844224) )
	ROM_LOAD16_BYTE( "hosh-52.bin", 0x000001, 0x020000, CRC(39b2798d) SHA1(9d66f99b4b3ffaa07018e4c70bb83903d89c0a50) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-5n1.bin", 0x000000, 0x020000, CRC(e478392f) SHA1(2c973dfc64e43aed2ec46fcb81fe1b50783daf68) )
	ROM_LOAD16_BYTE( "hosh-52.bin", 0x000001, 0x020000, CRC(39b2798d) SHA1(9d66f99b4b3ffaa07018e4c70bb83903d89c0a50) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hosh-5p1.bin", 0x000000, 0x020000, CRC(3ed0d5d3) SHA1(d04ca69a9e3b30a64c2bad1c4e263b2730ae3fc2) )
	ROM_LOAD16_BYTE( "hosh-52.bin", 0x000001, 0x020000, CRC(39b2798d) SHA1(9d66f99b4b3ffaa07018e4c70bb83903d89c0a50) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshth )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hotshot.p1", 0x00000, 0x020000, CRC(4a15d870) SHA1(518fe05ee3b7039827271659777535c143f6fcc1) )
	ROM_LOAD16_BYTE( "hotshot.p2", 0x00001, 0x020000, CRC(5b814e89) SHA1(214c8ed823089a2c53b53b6ce85863dc3cd38da2) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshti )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "044p3-0n.bin", 0x000000, 0x020000, CRC(02abba15) SHA1(8073da7ea300fe5b89e6b317c6e051171fccb4a1) )
	ROM_LOAD16_BYTE( "prom2_0.bin", 0x000001, 0x020000, CRC(8d89d7eb) SHA1(b69501f160d9980558a342e1d1a3e4102e9b9c33) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1p_0.bin", 0x000000, 0x020000, CRC(e935ee2d) SHA1(0d5abd6e3f5d04f5874015a158446d0598db8ec1) )
	ROM_LOAD16_BYTE( "prom2_0.bin", 0x000001, 0x020000, CRC(8d89d7eb) SHA1(b69501f160d9980558a342e1d1a3e4102e9b9c33) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1n.bin", 0x000000, 0x020000, CRC(df747d47) SHA1(68d28db3fc2bb806f88ee284a8051e15b32cf11e) )
	ROM_LOAD16_BYTE( "prom2_1.bin", 0x000001, 0x020000, CRC(22bf5681) SHA1(4f142dee31dc00e5ecf0a2305282766b368f4c33) )
	j6hotsht_sound
ROM_END

ROM_START( j6hotshtl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1p_1.bin", 0x000000, 0x020000, CRC(34ea297f) SHA1(2e7476a9dc4e87a219ed3ce0fbe7b861e8a4832e) )
	ROM_LOAD16_BYTE( "prom2_1.bin", 0x000001, 0x020000, CRC(22bf5681) SHA1(4f142dee31dc00e5ecf0a2305282766b368f4c33) )
	j6hotsht_sound
ROM_END


#define j6impuls_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "impu-snd.bin", 0x000000, 0x080000, CRC(0551d030) SHA1(7a8d012690bcea707710bf39c8069d7c074912ce) )

ROM_START( j6impuls )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "imp15p1", 0x000000, 0x020000, CRC(115aa0fc) SHA1(0ff0c1b87ba1c6e9d875857dcbf9e67174e86962) )
	ROM_LOAD16_BYTE( "imp15p2", 0x000001, 0x020000, CRC(f9b0a1f5) SHA1(0833ffe044cdefff7836fc00e985e5b0d7e9f827) )
	j6impuls_sound
ROM_END



ROM_START( j6impulsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "imp6p1", 0x000000, 0x020000, CRC(2abd3397) SHA1(36d6286d6de6c1b205ca1e593118bf5ef15e108b) )
	ROM_LOAD16_BYTE( "imp6p2", 0x000001, 0x020000, CRC(6d8ae792) SHA1(9e703f3e4c6e74866f8a6c187851c416905dc076) )
	j6impuls_sound
ROM_END

ROM_START( j6impulsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "impu6p1a", 0x000000, 0x020000, CRC(7879f3f6) SHA1(a7e5e55946ed63bf7fd84ac8cce46102a850bae4) )
	ROM_LOAD16_BYTE( "imp6p2", 0x000001, 0x020000, CRC(6d8ae792) SHA1(9e703f3e4c6e74866f8a6c187851c416905dc076) )
	j6impuls_sound
ROM_END

ROM_START( j6impulsc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "impu-7s1.bin", 0x000000, 0x020000, CRC(93033ab0) SHA1(6b94c72ba09a2b3bf343f199a61871f18b67ed10) )
	ROM_LOAD16_BYTE( "impu-7s2.bin", 0x000001, 0x020000, CRC(cead0007) SHA1(f5d701bd2f1d85fd907666d0fbe217dbeaae1ba7) )
	j6impuls_sound
ROM_END

ROM_START( j6impulsd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "impu-7a1.bin", 0x000000, 0x020000, CRC(c1c7fad1) SHA1(a28db0949be931192ebacf6dc67a1b652fd048c2) )
	ROM_LOAD16_BYTE( "impu-7s2.bin", 0x000001, 0x020000, CRC(cead0007) SHA1(f5d701bd2f1d85fd907666d0fbe217dbeaae1ba7) )
	j6impuls_sound
ROM_END

ROM_START( j6impulse )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "impu-7p1.bin", 0x000000, 0x020000, CRC(789d6e88) SHA1(0b65f00b6fed5fbdd89d9ba06ebe7d17a97344d6) )
	ROM_LOAD16_BYTE( "impu-7s2.bin", 0x000001, 0x020000, CRC(cead0007) SHA1(f5d701bd2f1d85fd907666d0fbe217dbeaae1ba7) )
	j6impuls_sound
ROM_END

ROM_START( j6impulsf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "impu-7wp.bin", 0x000000, 0x020000, CRC(1af01f00) SHA1(0d3fcaa1105a5dd00d4154027dcdac6b35eb2342) )
	ROM_LOAD16_BYTE( "impu-7s2.bin", 0x000001, 0x020000, CRC(cead0007) SHA1(f5d701bd2f1d85fd907666d0fbe217dbeaae1ba7) )
	j6impuls_sound
ROM_END


#define j6indy_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "6706.bin", 0x000000, 0x080000, CRC(674c5b21) SHA1(12e12e362ae8c99414cd474a34fa13acd8f6bcb3) ) \
	ROM_LOAD( "indisnd.bin", 0x0000, 0x080000, CRC(90ff139b) SHA1(9555553dc01055c311d4917e6ed7f5d3b6bf3b71) )

ROM_START( j6indy )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6810.bin", 0x000000, 0x020000, CRC(67f9cf6a) SHA1(f5e63b2135f9b251bb092e2738ab280581792a08) )
	ROM_LOAD16_BYTE( "6811.bin", 0x000001, 0x020000, CRC(6efc0ce8) SHA1(2f1bc1dfd6d1df019f180e6477e524811bf7295c) )
	j6indy_sound
ROM_END

ROM_START( j6indya )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6812.bin", 0x000000, 0x020000, CRC(8c679b52) SHA1(781ddd1a1cbfe96f0a7100a9aaa7a8d4641295fd) )
	ROM_LOAD16_BYTE( "6811.bin", 0x000001, 0x020000, CRC(6efc0ce8) SHA1(2f1bc1dfd6d1df019f180e6477e524811bf7295c) )
	j6indy_sound
ROM_END

ROM_START( j6indyb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6814.bin", 0x000000, 0x020000, CRC(6bb4615b) SHA1(df46a5fe7a4a24feb0422dcb9ca6c8a4a1d57cd2) )
	ROM_LOAD16_BYTE( "6811.bin", 0x000001, 0x020000, CRC(6efc0ce8) SHA1(2f1bc1dfd6d1df019f180e6477e524811bf7295c) )
	j6indy_sound
ROM_END

ROM_START( j6indyc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6823.bin", 0x000000, 0x020000, CRC(c0cf1be5) SHA1(0cd5189e896f2e000c9ce3a7a1d85cd17efc5f84) )
	ROM_LOAD16_BYTE( "6824.bin", 0x000001, 0x020000, CRC(191bf6c9) SHA1(d33c330eb6b7bdb3fef98a97a9a6af8ec6910a62) )
	j6indy_sound
ROM_END

ROM_START( j6indyd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6825.bin", 0x000000, 0x020000, CRC(2b514fdd) SHA1(732ce399720a7e4e2bc0daf226eb8a1d5a229531) )
	ROM_LOAD16_BYTE( "6824.bin", 0x000001, 0x020000, CRC(191bf6c9) SHA1(d33c330eb6b7bdb3fef98a97a9a6af8ec6910a62) )
	j6indy_sound
ROM_END

ROM_START( j6indye )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7038.bin", 0x000000, 0x020000, CRC(6e209529) SHA1(a134eb4f4a20f3ec5e71c1fc6d0fdef7a7358f4c) )
	ROM_LOAD16_BYTE( "7039.bin", 0x000001, 0x020000, CRC(32e6bfe4) SHA1(71e9030b0c2752df4a78d6f561549c3da6ce3ea3) )
	j6indy_sound
ROM_END

ROM_START( j6indyf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7040.bin", 0x000000, 0x020000, CRC(85bec111) SHA1(f259553c226cfbf43ab9d35a6e82a19db82405ce) )
	ROM_LOAD16_BYTE( "7039.bin", 0x000001, 0x020000, CRC(32e6bfe4) SHA1(71e9030b0c2752df4a78d6f561549c3da6ce3ea3) )
	j6indy_sound
ROM_END

ROM_START( j6indyg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7041.bin", 0x000000, 0x020000, CRC(3ce45548) SHA1(670c9e1a07db72510cd713f10639624334b3f386) )
	ROM_LOAD16_BYTE( "7039.bin", 0x000001, 0x020000, CRC(32e6bfe4) SHA1(71e9030b0c2752df4a78d6f561549c3da6ce3ea3) )
	j6indy_sound
ROM_END

ROM_START( j6indyh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7256.bin", 0x000000, 0x020000, CRC(626d3b18) SHA1(78ab605a755b63b95be5c2b4b48aa4eb282aeb69) )
	ROM_LOAD16_BYTE( "7039.bin", 0x000001, 0x020000, CRC(32e6bfe4) SHA1(71e9030b0c2752df4a78d6f561549c3da6ce3ea3) )
	j6indy_sound
ROM_END

ROM_START( j6indyi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7493.bin", 0x000000, 0x020000, CRC(21ca4ac6) SHA1(37c78df9dcace53eeab72ea37a49d27056d35043) )
	ROM_LOAD16_BYTE( "7494.bin", 0x000001, 0x020000, CRC(4ddf626b) SHA1(4bbfd86530cd1a8b7b4da4e9b36d0e1d61e5d120) )
	j6indy_sound
ROM_END

ROM_START( j6indyj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7495.bin", 0x000000, 0x020000, CRC(ca541efe) SHA1(9de0f2c2b486efc6283b11aafdf72c14f0c96eb7) )
	ROM_LOAD16_BYTE( "7494.bin", 0x000001, 0x020000, CRC(4ddf626b) SHA1(4bbfd86530cd1a8b7b4da4e9b36d0e1d61e5d120) )
	j6indy_sound
ROM_END

ROM_START( j6indyk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7496.bin", 0x000000, 0x020000, CRC(730e8aa7) SHA1(5dd684c8066c94b3bd25d71b7e0e4f11f4e9a9ac) )
	ROM_LOAD16_BYTE( "7494.bin", 0x000001, 0x020000, CRC(4ddf626b) SHA1(4bbfd86530cd1a8b7b4da4e9b36d0e1d61e5d120) )
	j6indy_sound
ROM_END




ROM_START( j6indyge )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "indy04040.p1", 0x00000, 0x10000, CRC(e80066ce) SHA1(6b6fcd1e1d307469031acd83a9acb7488536dccc) )
	ROM_LOAD16_BYTE( "indy04040.p2", 0x00001, 0x10000, CRC(ddf59c2c) SHA1(bbe523f11db98c6141d7a43beee98e4ec8af7408) )

	ROM_REGION( 0x80000, "upd", 0 )
	ROM_LOAD( "indy04020.snd", 0x00000, 0x080000, CRC(0266d48e) SHA1(c16ebdc6f73a6c832f765b909ca5511eaf728897) )
ROM_END


ROM_START( j6indyge2 )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "indy04020.p1", 0x00000, 0x10000, CRC(52ca267c) SHA1(7cd90b536d974afd504620f12868c5efe83ef9dd) )
	ROM_LOAD16_BYTE( "indy04020.p2", 0x00001, 0x10000, CRC(d96e3937) SHA1(902bb6021f5e1fa7471bce9de207e4df5f071fc4) )

	ROM_REGION( 0x80000, "upd", 0 )
	ROM_LOAD( "indy04020.snd", 0x00000, 0x080000, CRC(0266d48e) SHA1(c16ebdc6f73a6c832f765b909ca5511eaf728897) )
ROM_END

#define j6showtm_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "itsshowtimesnd.bin", 0x000000, 0x080000, CRC(7a2264fa) SHA1(55d5a15ff8c6a76c5403856bb8e64cbfdafb7a55) )

ROM_START( j6showtm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsshowtime.p1", 0x000000, 0x020000, CRC(5e744b6a) SHA1(db653618e4c2b86634bb10795bd6c3ad3a1b199e) )
	ROM_LOAD16_BYTE( "itsshowtime.p2", 0x000001, 0x020000, CRC(ae952cf9) SHA1(e27c616e74eb139daf98479fed0cc1bfcf5619b5) ) // aka its showtime v2-2 (27c010)
	j6showtm_sound
ROM_END

ROM_START( j6showtma )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "its showtime v2-1,27c010", 0x000000, 0x020000, CRC(b5ea1f52) SHA1(08d6bc1add1e659b59e2edc9de88dd976587ece3) )
	ROM_LOAD16_BYTE( "itsshowtime.p2", 0x000001, 0x020000, CRC(ae952cf9) SHA1(e27c616e74eb139daf98479fed0cc1bfcf5619b5) ) // aka its showtime v2-2 (27c010)
	j6showtm_sound
ROM_END


ROM_START( j6showtmb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "its showtime v1a1,27c010", 0x000000, 0x020000, CRC(c10295cf) SHA1(076f25a24a08e0d3657ac90f54a29af1e22603d9) )
	ROM_LOAD16_BYTE( "its showtime v1-2,27c010", 0x000001, 0x020000, CRC(e5db5a67) SHA1(6bdd8f48edb6c6dce9dc6ac742825b30e2efda40) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-41.bin", 0x000000, 0x020000, CRC(4f640b5f) SHA1(20a893d9a0ba10fc0bf63380e754e141d795759c) )
	ROM_LOAD16_BYTE( "itsh-42.bin", 0x000001, 0x020000, CRC(7a14783a) SHA1(804b1438cb5b3502fee0abf8a29174efc64e6dd6) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-4a1.bin", 0x000000, 0x020000, CRC(1da0cb3e) SHA1(30b7bf37cbd8c3372ae9ea1e114c4d7cc27ec131) )
	ROM_LOAD16_BYTE( "itsh-42.bin", 0x000001, 0x020000, CRC(7a14783a) SHA1(804b1438cb5b3502fee0abf8a29174efc64e6dd6) )
	j6showtm_sound
ROM_END

ROM_START( j6showtme )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-4n1.bin", 0x000000, 0x020000, CRC(7e52b39b) SHA1(d81a6d50c7596be53f7feaa4bf975978afb22dde) )
	ROM_LOAD16_BYTE( "itsh-42.bin", 0x000001, 0x020000, CRC(7a14783a) SHA1(804b1438cb5b3502fee0abf8a29174efc64e6dd6) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-4p1.bin", 0x000000, 0x020000, CRC(a4fa5f67) SHA1(b038b614ba62f765018b27f0180a11979d84f45b) )
	ROM_LOAD16_BYTE( "itsh-42.bin", 0x000001, 0x020000, CRC(7a14783a) SHA1(804b1438cb5b3502fee0abf8a29174efc64e6dd6) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh4np1.bin", 0x000000, 0x020000, CRC(95cce7a3) SHA1(6e848fae84a9a053a1aa91b5b9deb9c7f9ad5ebc) )
	ROM_LOAD16_BYTE( "itsh-42.bin", 0x000001, 0x020000, CRC(7a14783a) SHA1(804b1438cb5b3502fee0abf8a29174efc64e6dd6) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-61.bin", 0x000000, 0x020000, CRC(0385bc13) SHA1(d627fe6083bc0cec726a06e4237f1f2b34cc44b1) )
	ROM_LOAD16_BYTE( "itsh-62.bin", 0x000001, 0x020000, CRC(cc164769) SHA1(c0cbcf8bf4530882a46cfffa9e1b90eea30284e7) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-6a1.bin", 0x000000, 0x020000, CRC(51417c72) SHA1(261a7d90842f2d1b438655b8f694cef6b616a09f) )
	ROM_LOAD16_BYTE( "itsh-62.bin", 0x000001, 0x020000, CRC(cc164769) SHA1(c0cbcf8bf4530882a46cfffa9e1b90eea30284e7) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-6l1.bin", 0x000000, 0x020000, CRC(32b304d7) SHA1(beea0699c8dd4c4aefaee539b4ba93815803e2cb) )
	ROM_LOAD16_BYTE( "itsh-62.bin", 0x000001, 0x020000, CRC(cc164769) SHA1(c0cbcf8bf4530882a46cfffa9e1b90eea30284e7) )
	j6showtm_sound
ROM_END

ROM_START( j6showtmk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh-6p1.bin", 0x000000, 0x020000, CRC(e81be82b) SHA1(51738c89fd7017d52237085bf3a1f0dbcc0610b0) )
	ROM_LOAD16_BYTE( "itsh-62.bin", 0x000001, 0x020000, CRC(cc164769) SHA1(c0cbcf8bf4530882a46cfffa9e1b90eea30284e7) )
	j6showtm_sound
ROM_END

ROM_START( j6showtml )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "itsh6lp1.bin", 0x000000, 0x020000, CRC(d92d50ef) SHA1(e0d0f68890d271c1c30f0ac78ea86a37eda41afe) )
	ROM_LOAD16_BYTE( "itsh-62.bin", 0x000001, 0x020000, CRC(cc164769) SHA1(c0cbcf8bf4530882a46cfffa9e1b90eea30284e7) )
	j6showtm_sound
ROM_END


#define j6jackjs_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "jackpotjusticesnd.bin", 0x000000, 0x080000, CRC(379e1a3d) SHA1(3b455a812284e716a831aadbaa592ee0ddab1a9d) )
ROM_START( j6jackjs ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60000150.evn", 0x000000, 0x080000, CRC(358d59c8) SHA1(9212565a1f9d50d26d4ccfff747824c42e878e52) )
	ROM_LOAD16_BYTE( "60000150.odd", 0x000001, 0x080000, CRC(4a3473c8) SHA1(4ec3367008a8e1f34a7d502a9c9387d1b6de6e98) )
	j6jackjs_sound
ROM_END

ROM_START( j6jackjsa ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60001150.evn", 0x000000, 0x080000, CRC(0df9c631) SHA1(b205fbdf0bf59b604ff6ca4af7f41f5128db9d45) )
	ROM_LOAD16_BYTE( "60001150.odd", 0x000001, 0x080000, CRC(6605981a) SHA1(a94fa61ba1f9c4e95416837b5dfe55092ef8b290) )
	j6jackjs_sound
ROM_END

ROM_START( j6jackjsb ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60001151.evn", 0x000000, 0x080000, CRC(e13db919) SHA1(49426dfd450bbf0ffd80fb416d8ef91ac09e709d) )
	ROM_LOAD16_BYTE( "60001151.odd", 0x000001, 0x080000, CRC(d1157eac) SHA1(76504b803fa46b707e78bc50030e2dc9a64fc209) )
	j6jackjs_sound
ROM_END

ROM_START( j6jackjsc ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jj.p1", 0x000000, 0x080000, CRC(22ec7048) SHA1(7e806963d0aeccc1c92579eebf2571dd9f29c263) )
	ROM_LOAD16_BYTE( "jj.p2", 0x000001, 0x080000, CRC(a54742da) SHA1(61f1b888c60cf157b07f853b52f644acc82c283d) )
	j6jackjs_sound
ROM_END


// different startup code
ROM_START( j6jkrgld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jg.p1", 0x000000, 0x010000, CRC(e5658ca2) SHA1(2d188899a4aa8124b7c492379331b8713913c69e) )
	ROM_LOAD16_BYTE( "jg.p2", 0x000001, 0x010000, CRC(efa0c84b) SHA1(ef511378904823ae66b7812eff13d9cef5fa621b) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END


#define j6jkrpls_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */
ROM_START( j6jkrpls )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jp30cz04_1.b8", 0x000000, 0x020000, CRC(096baa03) SHA1(d9d3aa5616e253b49adda9254dbdaedb3e7ee72a) )
	ROM_LOAD16_BYTE( "jp30cz04_2.b8", 0x000001, 0x020000, CRC(bc023fe9) SHA1(99e5cfca3788809bf1958d21bfff37693419b846) ) // aka jp75cz04_2.b8
	j6jkrpls_sound
ROM_END

ROM_START( j6jkrplsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jp75cz04_1.b8", 0x000000, 0x020000, CRC(05260432) SHA1(3c68f1476be4803d901abc4e6f7bb81b14de4442) )
	ROM_LOAD16_BYTE( "jp30cz04_2.b8", 0x000001, 0x020000, CRC(bc023fe9) SHA1(99e5cfca3788809bf1958d21bfff37693419b846) ) // aka jp75cz04_2.b8
	j6jkrpls_sound
ROM_END

ROM_START( j6jkrplsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jp30cz3_1.b8", 0x000000, 0x020000, CRC(939729ba) SHA1(b5a142c5e33755faebf7de065fbab1dbe9b78387) )
	ROM_LOAD16_BYTE( "jp30cz3_2.b8", 0x000001, 0x020000, CRC(8dcf654b) SHA1(076b1454b6bb4c634cb3b08922e49b7ae4f7bc32) ) // aka jp75cz3_2.b8
	j6jkrpls_sound
ROM_END

ROM_START( j6jkrplsc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jp75cz3_1.b8", 0x000000, 0x020000, CRC(9fda878b) SHA1(e57944bff15d2be16c241461afb7e7b69a0ded0f) )
	ROM_LOAD16_BYTE( "jp30cz3_2.b8", 0x000001, 0x020000, CRC(8dcf654b) SHA1(076b1454b6bb4c634cb3b08922e49b7ae4f7bc32) ) // aka jp75cz3_2.b8
	j6jkrpls_sound
ROM_END

ROM_START( j6jkrplsd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jp30cz05_1.b8", 0x000000, 0x020000, CRC(d36a7516) SHA1(613508c57c1e4e9ebfbdb1516d90cfa4e650721f) )
	ROM_LOAD16_BYTE( "jp30cz05_2.b8", 0x000000, 0x020000, CRC(233f2cd8) SHA1(edd02ebc4612075d9c594a5c37ff595bc695edd3) ) // aka jp75cz05_2.b8
	j6jkrpls_sound
ROM_END

ROM_START( j6jkrplse )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jp75cz05_1.b8", 0x000000, 0x020000, CRC(df27db27) SHA1(4d0d1673ffaa506f442e1d0a69ee05693ec901ae) )
	ROM_LOAD16_BYTE( "jp30cz05_2.b8", 0x000000, 0x020000, CRC(233f2cd8) SHA1(edd02ebc4612075d9c594a5c37ff595bc695edd3) ) // aka jp75cz05_2.b8
	j6jkrpls_sound
ROM_END



#define j6jkpldx_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */
ROM_START( j6jkpldx )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jpd75cz03_1.b8", 0x000000, 0x020000, CRC(f48c26c5) SHA1(5a46a24a4575da3360eab54059ea994b7e8e4f8d) )
	ROM_LOAD16_BYTE( "jpd75cz03_2.b8", 0x000001, 0x020000, CRC(02d31498) SHA1(feb345442354b15a1a0a86e6b86db519aa8678fa) )
	j6jkpldx_sound
ROM_END


ROM_START( j6jkpldxa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jpc75cz03_1.b8", 0x000000, 0x020000, CRC(efc89c1b) SHA1(b532e035e670dea515503b57c872640f5f39b114) )
	ROM_LOAD16_BYTE( "jpc75cz03_2.b8", 0x000001, 0x020000, CRC(8cac4066) SHA1(6950df8e9fa3cef3f1f5476c5290a9ff1308636a) )
	j6jkpldx_sound
ROM_END


ROM_START( j6jkwld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jpm_jokerswild_1001_1.bin", 0x000000, 0x020000, CRC(1a38aca0) SHA1(59a4d547cd22af3b5e5eaa161b564616ddcbc5ee) )
	ROM_LOAD16_BYTE( "jpm_jokerswild_1001_2.bin", 0x000001, 0x020000, CRC(a29a1961) SHA1(94e9af886609f4f0abe87d3ba71f0b533052ff2c) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "jokwild.snd", 0x000000, 0x080000, CRC(352e28cd) SHA1(c98307f5eaf511c9d281151d1c07ffd83f24244c) )
ROM_END


ROM_START( j6jungfv )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "jun12nsk.1", 0x000000, 0x020000, CRC(4c867372) SHA1(6a16170f1e76ba4b49de5a963cba072b5934d23f) )
	ROM_LOAD16_BYTE( "jun12.2",    0x000001, 0x020000, CRC(4c755c51) SHA1(69df321acabed9fc83aa3137b9dafe22064568cd) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END


ROM_START( j6kamel )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "kameleon1.bin", 0x000000, 0x020000, CRC(d67063b5) SHA1(b48d21e7783812aae2ac6765dcef68287a3916ee) )
	ROM_LOAD16_BYTE( "kameleon2.bin", 0x000001, 0x020000, CRC(d7323d2f) SHA1(44c56c24c3fee291344eaacfb0183ec7f06b9cf7) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "kameleon.snd", 0x000000, 0x02c3f8, CRC(c294e7aa) SHA1(952fd4fd7e61125b5f8bbe6585d2aa3ca3eda605) )
ROM_END


#define j6kungfu_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "kufu-snd.bin", 0x000000, 0x080000, CRC(95360279) SHA1(f86c5ef3f7e790e3062ebda5150d2384ea341651) )
ROM_START( j6kungfu )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "kfu20p1", 0x000000, 0x020000, CRC(f8dc50fb) SHA1(8beca856ac604d568e162d26c83f1d2984eccd6d) )
	ROM_LOAD16_BYTE( "kfu20p2", 0x000001, 0x020000, CRC(98d274ef) SHA1(9706f73a05f6ab1cda06afce8f814b19a768c646) )
	j6kungfu_sound
ROM_END

ROM_START( j6kungfua )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20067.bin", 0x000000, 0x020000, CRC(027d3728) SHA1(307456c5761e17ce2bc1d095c366f3aac92b77fb) )
	ROM_LOAD16_BYTE( "20068.bin", 0x000001, 0x020000, CRC(bdd0e987) SHA1(b6a2a3eb6b73c60c32373710653cb9da90e1a681) )
	j6kungfu_sound
ROM_END

ROM_START( j6kungfub )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20069.bin", 0x000000, 0x020000, CRC(e9e36310) SHA1(d36895d9c375c985daadd91159d67168d50936f2) )
	ROM_LOAD16_BYTE( "20068.bin", 0x000001, 0x020000, CRC(bdd0e987) SHA1(b6a2a3eb6b73c60c32373710653cb9da90e1a681) )
	j6kungfu_sound
ROM_END

ROM_START( j6kungfuc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20070.bin", 0x000000, 0x020000, CRC(50b9f749) SHA1(c5796803185cec1557705cb815547f882ded53c6) )
	ROM_LOAD16_BYTE( "20068.bin", 0x000001, 0x020000, CRC(bdd0e987) SHA1(b6a2a3eb6b73c60c32373710653cb9da90e1a681) )
	j6kungfu_sound
ROM_END

ROM_START( j6kungfud )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20071.bin", 0x000000, 0x020000, CRC(601046a0) SHA1(7e544f3734e8ccdf1c8bb475a98c4d82641dbfca) )
	ROM_LOAD16_BYTE( "20068.bin", 0x000001, 0x020000, CRC(bdd0e987) SHA1(b6a2a3eb6b73c60c32373710653cb9da90e1a681) )
	j6kungfu_sound
ROM_END



#define j6luckla_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6luckla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "llv-b1.bin", 0x000000, 0x020000, CRC(b926235e) SHA1(839fae5355a1e9ed24e7cb0a70c773463a04cab5) )
	ROM_LOAD16_BYTE( "llv-b2.bin", 0x000001, 0x020000, CRC(8bab8906) SHA1(ae83bf8d87ce664f35446392b28bd89e92113dbb) )
	j6luckla_sound
ROM_END


ROM_START( j6lucklaa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "llv-bf1.bin", 0x000000, 0x020000, CRC(7dfcc04e) SHA1(9d5e1c0265a2e7cb4090eab61cbb313104038fce) )
	ROM_LOAD16_BYTE( "llv-b2.bin", 0x000001, 0x020000, CRC(8bab8906) SHA1(ae83bf8d87ce664f35446392b28bd89e92113dbb) )
	j6luckla_sound
ROM_END

ROM_START( j6lucklab )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "llv-bp1.bin", 0x000000, 0x020000, CRC(52b87766) SHA1(2bab5d020992c7150e08bcb2b2da229a5216d5a4) )
	ROM_LOAD16_BYTE( "llv-b2.bin", 0x000001, 0x020000, CRC(8bab8906) SHA1(ae83bf8d87ce664f35446392b28bd89e92113dbb) )
	j6luckla_sound
ROM_END

#define j6magcir_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "f098.bin", 0x000000, 0x080000, CRC(a4431105) SHA1(2dad84011ccf08be5b642884b2353718ebb4a6c7) )
ROM_START( j6magcir )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "magic1.bin", 0x000000, 0x020000, CRC(c7a646dc) SHA1(c4c6e2ecccdccb66421a4c926b9cac5260f855e3) )
	ROM_LOAD16_BYTE( "magic2.bin", 0x000001, 0x020000, CRC(fc4c700b) SHA1(a25900062b531956420394a412d9b08f1ef2bd02) )
	j6magcir_sound
ROM_END

ROM_START( j6magcira )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "927f", 0x000000, 0x020000, CRC(622fc61e) SHA1(c81fcc062c101d5895c54f772d70a31facca3c89) )
	ROM_LOAD16_BYTE( "magic2.bin", 0x000001, 0x020000, CRC(fc4c700b) SHA1(a25900062b531956420394a412d9b08f1ef2bd02) )
	j6magcir_sound
ROM_END

ROM_START( j6magcirb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "92df", 0x000000, 0x020000, CRC(037ca5cc) SHA1(01e9608d912c6ee416bdcbe70001c3333119e26e) )
	ROM_LOAD16_BYTE( "magic2.bin", 0x000001, 0x020000, CRC(fc4c700b) SHA1(a25900062b531956420394a412d9b08f1ef2bd02) )
	j6magcir_sound
ROM_END

ROM_START( j6magcirc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "92f7", 0x000000, 0x020000, CRC(f690fe18) SHA1(f392c48d8693fd822967b5515190bec39410a379) )
	ROM_LOAD16_BYTE( "magic2.bin", 0x000001, 0x020000, CRC(fc4c700b) SHA1(a25900062b531956420394a412d9b08f1ef2bd02) )
	j6magcir_sound
ROM_END

ROM_START( j6magcird )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "92fe", 0x000000, 0x020000, CRC(2c3812e4) SHA1(8b3fac6f854eb0a4807b77a3626f72eff37ebc45) )
	ROM_LOAD16_BYTE( "magic2.bin", 0x000001, 0x020000, CRC(fc4c700b) SHA1(a25900062b531956420394a412d9b08f1ef2bd02) )
	j6magcir_sound
ROM_END

#define j6mavrk_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1431.bin", 0x000000, 0x080000, CRC(2c95a586) SHA1(81f27d408f29bec0c79a7ac635e74a11cc93f2cc) )

ROM_START( j6mavrk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9435.bin", 0x000000, 0x020000, CRC(b89e31a6) SHA1(9661f14fa9f655ac9748c67802755815da6a688e) )
	ROM_LOAD16_BYTE( "9436.bin", 0x000001, 0x020000, CRC(cbd565db) SHA1(afdb708a0724746feefde39dd202aaf4250a039b) )
	j6mavrk_sound
ROM_END

ROM_START( j6mavrka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9437.bin", 0x000000, 0x020000, CRC(5300659e) SHA1(f5be69a4abce517b4d00877ed8ffbf4ed6fc62ac) )
	ROM_LOAD16_BYTE( "9436.bin", 0x000001, 0x020000, CRC(cbd565db) SHA1(afdb708a0724746feefde39dd202aaf4250a039b) )
	j6mavrk_sound
ROM_END

ROM_START( j6mavrkb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9438.bin", 0x000000, 0x020000, CRC(ea5af1c7) SHA1(c004d418801e1caf239df4fdaf44ec1382988eea) )
	ROM_LOAD16_BYTE( "9436.bin", 0x000001, 0x020000, CRC(cbd565db) SHA1(afdb708a0724746feefde39dd202aaf4250a039b) )
	j6mavrk_sound
ROM_END

ROM_START( j6mavrkc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9439.bin", 0x000000, 0x020000, CRC(daf3402e) SHA1(9b57ea4649b4b198c59d49b2be2541dce00fc6af) )
	ROM_LOAD16_BYTE( "9436.bin", 0x000001, 0x020000, CRC(cbd565db) SHA1(afdb708a0724746feefde39dd202aaf4250a039b) )
	j6mavrk_sound
ROM_END

ROM_START( j6mavrkd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mav4p1.bin", 0x000000, 0x020000, CRC(d341b640) SHA1(8bdeb5ef07de5179685862ea82b33b337ed40055) )
	ROM_LOAD16_BYTE( "mav4_p2.bin", 0x000001, 0x020000, CRC(9813901e) SHA1(5e8e6565632ef6c0d1ee2ce9a5d68cb1de096d0c) )
	j6mavrk_sound
ROM_END

#define j6maxod_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "maov-snd.bin", 0x000000, 0x080000, CRC(9b527476) SHA1(6a6333aea592a1a7331a79372bbd6a16ff35c252) )

ROM_START( j6maxod )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "maov-4s1.bin", 0x000000, 0x020000, CRC(f7b64d04) SHA1(1e534125c499a6c5bee1a84287c30caac06dd1fd) )
	ROM_LOAD16_BYTE( "maov-4s2.bin", 0x000001, 0x020000, CRC(b15d7a78) SHA1(77c14983a9ed9a58a183202390070a28f746f0a7) )
	j6maxod_sound
ROM_END

ROM_START( j6maxoda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "maov-4a1.bin", 0x000000, 0x020000, CRC(a5728d65) SHA1(55a25f8fc3c372c324b5eb3105e6b0a249481672) )
	ROM_LOAD16_BYTE( "maov-4s2.bin", 0x000001, 0x020000, CRC(b15d7a78) SHA1(77c14983a9ed9a58a183202390070a28f746f0a7) )
	j6maxod_sound
ROM_END

ROM_START( j6maxodb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "maov-4p1.bin", 0x000000, 0x020000, CRC(1c28193c) SHA1(6788344909819863e624974b8a2b60b0f5d4e235) )
	ROM_LOAD16_BYTE( "maov-4s2.bin", 0x000001, 0x020000, CRC(b15d7a78) SHA1(77c14983a9ed9a58a183202390070a28f746f0a7) )
	j6maxod_sound
ROM_END

ROM_START( j6maxodc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "maov-4w1.bin", 0x000000, 0x020000, CRC(95db3c8c) SHA1(bd5a7101ec7b04187178a5fec882a2ea5264c97f) )
	ROM_LOAD16_BYTE( "maov-4s2.bin", 0x000001, 0x020000, CRC(b15d7a78) SHA1(77c14983a9ed9a58a183202390070a28f746f0a7) )
	j6maxod_sound
ROM_END



// startup code very different here.. but still makes the same 4800a0 write in the end
ROM_START( j6maxcsh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "maxcash.p1", 0x000000, 0x020000, CRC(814459c1) SHA1(3584a772cefb252039d64f3350cfad88407dcc04) )
	ROM_LOAD16_BYTE( "maxcash.p2", 0x000001, 0x020000, CRC(6492e093) SHA1(ccbf8e997f282f914311e7fbb33bfe9088a38b2e) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "maxcashsnd.bin", 0x000000, 0x080000, CRC(bfa9b42e) SHA1(2dae055df7571b4fd5feed55900f3873cfb00719) )
ROM_END

#define j6medal_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "medjob.bin", 0x000000, 0x080000, CRC(dd8296bf) SHA1(c9209abf4276d81897476420177d24e739f0441e) )

ROM_START( j6medal ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60000101.evn", 0x000000, 0x080000, CRC(c26a8f9c) SHA1(d8ab81ee2c3e00016f215c68d3bc77b8cb3b5cf5) )
	ROM_LOAD16_BYTE( "60000101.odd", 0x000001, 0x080000, CRC(acf2ec0a) SHA1(b5d0b586c486de0185aa2c7fc991a5f13cebd9a7) )
	j6medal_sound
ROM_END

ROM_START( j6medala ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60000103.evn", 0x000000, 0x080000, CRC(1ca919ac) SHA1(72df757a7f04b0224d84032ac8ca39caa40f6b67) )
	ROM_LOAD16_BYTE( "60000103.odd", 0x000001, 0x080000, CRC(7cba5d94) SHA1(b0f2dd057163879a653cae9270feece8570849ce) )
	j6medal_sound
ROM_END

ROM_START( j6medalb ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60000104.evn", 0x000000, 0x080000, CRC(96389082) SHA1(03ef585ef3dc2c612d688a47399ac6b7fcd0d562) )
	ROM_LOAD16_BYTE( "60000104.odd", 0x000001, 0x080000, CRC(1add99b1) SHA1(228ef281ff45610dee999975c2aa51eb95ac4a29) )
	j6medal_sound
ROM_END

ROM_START( j6medalc ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60001103.evn", 0x000000, 0x080000, CRC(1bf17077) SHA1(f14a0d1f255ab9665ac8ea4e02cdbc4178396b55) )
	ROM_LOAD16_BYTE( "60001103.odd", 0x000001, 0x080000, CRC(66dcc1da) SHA1(3cf289e9c44b24cfb3a74ec810f3cbd4b7bdb891) )
	j6medal_sound
ROM_END

ROM_START( j6medald ) // not overdumps? legit larger game?
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60001104.evn", 0x000000, 0x080000, CRC(a3955286) SHA1(19f864dc04b158882322b1079d40d0588a41b517) )
	ROM_LOAD16_BYTE( "60001104.odd", 0x000001, 0x080000, CRC(36ec7263) SHA1(d554b702ffd6af0c7c8b700f8e09d348b387598d) )
	j6medal_sound
ROM_END


#define j6megbck_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "mebu-snd.bin", 0x000000, 0x080000, CRC(20bce62c) SHA1(50c5959eb5a5f8436a08f9a6a096b18cbf49970e) )
ROM_START( j6megbck )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mebu-4_1.bin", 0x000000, 0x020000, CRC(85080234) SHA1(9a65e7adbf4f5f4832f7daebc2ff9abd430b74a2) )
	ROM_LOAD16_BYTE( "mebu-4_2.bin", 0x000001, 0x020000, CRC(4f34f862) SHA1(918f056a394c415d45b9a2d4cbe24b7d0ff531f7) )
	j6megbck_sound
ROM_END

ROM_START( j6megbcka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mebu-4a1.bin", 0x000000, 0x020000, CRC(d7ccc255) SHA1(337be895b7af3222bf05b0dd36701684f118afac) )
	ROM_LOAD16_BYTE( "mebu-4_2.bin", 0x000001, 0x020000, CRC(4f34f862) SHA1(918f056a394c415d45b9a2d4cbe24b7d0ff531f7) )
	j6megbck_sound
ROM_END

ROM_START( j6megbckb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mebu-4n1.bin", 0x000000, 0x020000, CRC(b43ebaf0) SHA1(08af12e11e20d37a95528e43b0a50dc5a7e657fa) )
	ROM_LOAD16_BYTE( "mebu-4_2.bin", 0x000001, 0x020000, CRC(4f34f862) SHA1(918f056a394c415d45b9a2d4cbe24b7d0ff531f7) )
	j6megbck_sound
ROM_END

ROM_START( j6megbckc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mebu-4p1.bin", 0x000000, 0x020000, CRC(6e96560c) SHA1(af35cc8fda76100776312f4c53c1c15a508b175a) )
	ROM_LOAD16_BYTE( "mebu-4_2.bin", 0x000001, 0x020000, CRC(4f34f862) SHA1(918f056a394c415d45b9a2d4cbe24b7d0ff531f7) )
	j6megbck_sound
ROM_END

ROM_START( j6megbckd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mebu-4w1.bin", 0x000000, 0x020000, CRC(e76573bc) SHA1(58b65f7423d66d9ba839a450dd40100b4bfc6ffd) )
	ROM_LOAD16_BYTE( "mebu-4_2.bin", 0x000001, 0x020000, CRC(4f34f862) SHA1(918f056a394c415d45b9a2d4cbe24b7d0ff531f7) )
	j6megbck_sound
ROM_END




ROM_START( j6monmad )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mad14dsk.1", 0x000000, 0x020000, CRC(7b092a92) SHA1(808fd662ff5f4f02fb71b7864ab1951598847948) )
	ROM_LOAD16_BYTE( "mad14.2",    0x000001, 0x020000, CRC(f45f4187) SHA1(f571ad9174b2aab6a8bf18ee7cf768ce19e41339) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END




#define j6montlk_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
/*  ROM_LOAD( "1109.bin", 0x000000, 0x080000, CRC(b4d7ac12) SHA1(ac194d15e9d4e5cdadddbf2dc3c9660b52f116c2) ) */  \
	ROM_LOAD( "mtsnd.bin", 0x000000, 0x080000, CRC(b4d7ac12) SHA1(ac194d15e9d4e5cdadddbf2dc3c9660b52f116c2) )

ROM_START( j6montlk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "money talks 8 3-1.bin", 0x000000, 0x020000, CRC(42936de2) SHA1(e9ba9f2af8a6e2bcf887976f50ff7e3e3dcf86d7) )
	ROM_LOAD16_BYTE( "money talks 8 3-2.bin", 0x000001, 0x020000, CRC(25ad84ed) SHA1(f704eceadc6f4f422f6ff421837aa6c9f2f533f7) )
	j6montlk_sound
ROM_END

ROM_START( j6montlka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mtalk8ac", 0x000000, 0x020000, CRC(4edec3d3) SHA1(fa05a84819f4fc7f128b149ea4fcd6edee6d3b78) ) // aka money talks 8 3i1.bin
	ROM_LOAD16_BYTE( "money talks 8 3-2.bin", 0x000001, 0x020000, CRC(25ad84ed) SHA1(f704eceadc6f4f422f6ff421837aa6c9f2f533f7) )
	j6montlk_sound
ROM_END

ROM_START( j6montlkb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6844.bin", 0x000000, 0x020000, CRC(9d25282a) SHA1(4e8205a85853ed99e3568976edf95601e8b35599) )
	ROM_LOAD16_BYTE( "6845.bin", 0x000001, 0x020000, CRC(b747c0d4) SHA1(4bdabdf6156fdf588f5f55447f120ef5bf19621f) )
	j6montlk_sound
ROM_END

ROM_START( j6montlkc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6846.bin", 0x000000, 0x020000, CRC(76bb7c12) SHA1(a0f025a90e10cfce8bbf4c2671b30fb3ede12ba0) )
	ROM_LOAD16_BYTE( "6845.bin", 0x000001, 0x020000, CRC(b747c0d4) SHA1(4bdabdf6156fdf588f5f55447f120ef5bf19621f) )
	j6montlk_sound
ROM_END

ROM_START( j6montlkd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6847.bin", 0x000000, 0x020000, CRC(cfe1e84b) SHA1(275bf18954e706a98371d463a9372f3b5e7310f3) )
	ROM_LOAD16_BYTE( "6845.bin", 0x000001, 0x020000, CRC(b747c0d4) SHA1(4bdabdf6156fdf588f5f55447f120ef5bf19621f) )
	j6montlk_sound
ROM_END

ROM_START( j6montlke )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6848.bin", 0x000000, 0x020000, CRC(ff4859a2) SHA1(23bd372d6899eafc423afbee00c14954b8a37477) )
	ROM_LOAD16_BYTE( "6845.bin", 0x000001, 0x020000, CRC(b747c0d4) SHA1(4bdabdf6156fdf588f5f55447f120ef5bf19621f) )
	j6montlk_sound
ROM_END

ROM_START( j6montlkf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8299.bin", 0x000000, 0x020000, CRC(360cdf5a) SHA1(947ef29e101539515c321058712bcf685909f6d6) )
	ROM_LOAD16_BYTE( "8300.bin", 0x000001, 0x020000, CRC(71650174) SHA1(6907f82e54549fe6ecc1fd9f5d342b41357578a8) )
	j6montlk_sound
ROM_END

ROM_START( j6montlkg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8301.bin", 0x000000, 0x020000, CRC(dd928b62) SHA1(f79652160c23700c88b254120b5ba3ddc3ca6cf3) )
	ROM_LOAD16_BYTE( "8300.bin", 0x000001, 0x020000, CRC(71650174) SHA1(6907f82e54549fe6ecc1fd9f5d342b41357578a8) )
	j6montlk_sound
ROM_END

ROM_START( j6montlkh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8302.bin", 0x000000, 0x020000, CRC(64c81f3b) SHA1(0572f8c962432b08edd442512dcbc3b1461a4ede) )
	ROM_LOAD16_BYTE( "8300.bin", 0x000001, 0x020000, CRC(71650174) SHA1(6907f82e54549fe6ecc1fd9f5d342b41357578a8) )
	j6montlk_sound
ROM_END


#define j6mono60_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "monop60.snd", 0x000000, 0x080000, CRC(c79af6d0) SHA1(518a7b16978a843bdb83938279b11f446503361e) ) /* aka mon60_snd.bin */

ROM_START( j6mono60 )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9362.bin", 0x000000, 0x020000, CRC(b90825e1) SHA1(526399eb77f876f2946b8468ca2f980b66b0d739) )
	ROM_LOAD16_BYTE( "9363.bin", 0x000001, 0x020000, CRC(0eba908b) SHA1(29bd7dc6000004039037173f6098e52f20931b1e) )
	j6mono60_sound
ROM_END

ROM_START( j6mono60a )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9364.bin", 0x000000, 0x020000, CRC(529671d9) SHA1(1807b56e63d0d82fd03a72cfc9928b5ba9cbc0b7) )
	ROM_LOAD16_BYTE( "9363.bin", 0x000001, 0x020000, CRC(0eba908b) SHA1(29bd7dc6000004039037173f6098e52f20931b1e) )
	j6mono60_sound
ROM_END

ROM_START( j6mono60b )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9365.bin", 0x000000, 0x020000, CRC(ebcce580) SHA1(3323d93e7eb9ea54c79f050326d4ea77f470dc4f) )
	ROM_LOAD16_BYTE( "9363.bin", 0x000001, 0x020000, CRC(0eba908b) SHA1(29bd7dc6000004039037173f6098e52f20931b1e) )
	j6mono60_sound
ROM_END

ROM_START( j6mono60c )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9366.bin", 0x000000, 0x020000, CRC(db655469) SHA1(ef6924063384a85b6d3c6503f3ca8833076cbdb6) )
	ROM_LOAD16_BYTE( "9363.bin", 0x000001, 0x020000, CRC(0eba908b) SHA1(29bd7dc6000004039037173f6098e52f20931b1e) )
	j6mono60_sound
ROM_END

ROM_START( j6mono60d )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mo60.p1", 0x000000, 0x020000, CRC(1edce0e2) SHA1(626055d1df8a8ee2f9c650cee6cfeb047699e532) ) // aka mon60_p1.bin
	ROM_LOAD16_BYTE( "mo60.p2", 0x000001, 0x020000, CRC(1a1ea1da) SHA1(25e14d7ae82888e1b14d0dfb391d2dbafdb1b643) ) // aka mon60_p2.bin
	j6mono60_sound
ROM_END

ROM_START( j6mono60e )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8305.bin", 0x000000, 0x020000, CRC(f542b4da) SHA1(821be9da2c5f83f7c833eb729a136723b680905a) )
	ROM_LOAD16_BYTE( "mo60.p2", 0x000001, 0x020000, CRC(1a1ea1da) SHA1(25e14d7ae82888e1b14d0dfb391d2dbafdb1b643) ) // aka mon60_p2.bin
	j6mono60_sound
ROM_END

ROM_START( j6mono60f )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8306.bin", 0x000000, 0x020000, CRC(4c182083) SHA1(6647236b6511c5292bf6159713d61be8faa2597f) )
	ROM_LOAD16_BYTE( "mo60.p2", 0x000001, 0x020000, CRC(1a1ea1da) SHA1(25e14d7ae82888e1b14d0dfb391d2dbafdb1b643) ) // aka mon60_p2.bin
	j6mono60_sound
ROM_END

ROM_START( j6mono60g )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8307.bin", 0x000000, 0x020000, CRC(7cb1916a) SHA1(861e43efbaa28c5e8fc9f509173e855385c96131) )
	ROM_LOAD16_BYTE( "mo60.p2", 0x000001, 0x020000, CRC(1a1ea1da) SHA1(25e14d7ae82888e1b14d0dfb391d2dbafdb1b643) ) // aka mon60_p2.bin
	j6mono60_sound
ROM_END

ROM_START( j6mono60h )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8308.bin", 0x000000, 0x020000, CRC(12914ed3) SHA1(21d5c3cab4d51a4ae623ba1c407edbee282a485a) )
	ROM_LOAD16_BYTE( "mo60.p2", 0x000001, 0x020000, CRC(1a1ea1da) SHA1(25e14d7ae82888e1b14d0dfb391d2dbafdb1b643) ) // aka mon60_p2.bin
	j6mono60_sound
ROM_END

ROM_START( j6mono60i )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8659.bin", 0x000000, 0x020000, CRC(f90f1aeb) SHA1(ec21a7f6be4d9c5df3631ea8e42e1a6dbc63174d) )
	ROM_LOAD16_BYTE( "mo60.p2", 0x000001, 0x020000, CRC(1a1ea1da) SHA1(25e14d7ae82888e1b14d0dfb391d2dbafdb1b643) ) // aka mon60_p2.bin
	j6mono60_sound
ROM_END

ROM_START( j6mono60j )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "monov8p1", 0x000000, 0x020000, CRC(948ae4a2) SHA1(fdf4cb950d2637d5c8c18cb2709c8c5265609425) )
	ROM_LOAD16_BYTE( "monov8p2", 0x000001, 0x020000, CRC(16379f37) SHA1(65ed96cbbd816d5ae07b489981a50f445c6b07f4) )
	j6mono60_sound
ROM_END

ROM_START( j6mono60k )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "mon608p1", 0x000000, 0x020000, CRC(418ef9f2) SHA1(0bd2296f9b59a97befa9b87b967d21dac049f91c) )
	ROM_LOAD16_BYTE( "mon608p2", 0x000001, 0x00e000, BAD_DUMP CRC(f30f2300) SHA1(ba9e8ccbdebc0fb0f656b44886b6bee89d019b39) ) // wrong size
	j6mono60_sound
ROM_END

// different sound rom..
ROM_START( j6mono60l )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "s60_pound8p1", 0x00000, 0x020000, CRC(f5220142) SHA1(2d9460fbd833b477f579f0925dd999fa3d4355cc) )
	ROM_LOAD16_BYTE( "s60_pound8p2", 0x00001, 0x020000, CRC(3786bbb8) SHA1(d45d1d6713e480fa6e430fd1900778dcc56250c9) )
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "s60snd", 0x0000, 0x080000, CRC(f597a454) SHA1(ab88779657524df2b04b91ed35f25dc9206a5623) )
ROM_END





#define j6outlaw_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "outlawsnd.bin", 0x000000, 0x080000, CRC(9b9f21dc) SHA1(aca23a525f1288f49a18a74eb36ac3a67efa7e20) )
ROM_START( j6outlaw )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7547.bin", 0x000000, 0x020000, CRC(7a4efbf1) SHA1(5e184e172a129aad6ad34409f63de25916414146) )
	ROM_LOAD16_BYTE( "7548.bin", 0x000001, 0x020000, CRC(e5d61efc) SHA1(2e3ce747b14341ad5fcbf815a6b7e9a38a59478a) )
	j6outlaw_sound
ROM_END

ROM_START( j6outlawd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7549.bin", 0x000000, 0x020000, CRC(91d0afc9) SHA1(267500478d8fb73e61a869e53b598d0bea3c3caa) )
	ROM_LOAD16_BYTE( "7548.bin", 0x000001, 0x020000, CRC(e5d61efc) SHA1(2e3ce747b14341ad5fcbf815a6b7e9a38a59478a) )
	j6outlaw_sound
ROM_END

ROM_START( j6outlawc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "outlaw.p1", 0x000000, 0x020000, CRC(42b7d388) SHA1(4d647879a95b27788ea87885f266272344e910ea) )
	ROM_LOAD16_BYTE( "outlaw.p2", 0x000001, 0x020000, CRC(4c06f95a) SHA1(42085ea0c10930a27f0b223a6f0742165ee85727) )
	j6outlaw_sound
ROM_END

#define j6oxo_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "oxo_bingo_snd.bin", 0x000000, 0x080000, CRC(008a2d6a) SHA1(a89114154489142556b373ab24cd32fadf5856b3) )
ROM_START( j6oxo )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7613.bin", 0x000000, 0x020000, CRC(0621762a) SHA1(ec09cfed79158b09093a162785b6bdd1916ce50c) )
	ROM_LOAD16_BYTE( "7614.bin", 0x000001, 0x020000, CRC(f3bc7c8b) SHA1(a46928b9c5c7c14c5c555fb893a528763cd35963) )
	j6oxo_sound
ROM_END

ROM_START( j6oxoa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7615.bin", 0x000000, 0x020000, CRC(54e5b64b) SHA1(280f8b9f3fa871511d77c6cce0e836fa30b3bfc6) )
	ROM_LOAD16_BYTE( "7614.bin", 0x000001, 0x020000, CRC(f3bc7c8b) SHA1(a46928b9c5c7c14c5c555fb893a528763cd35963) )
	j6oxo_sound
ROM_END

ROM_START( j6oxob )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7616.bin", 0x000000, 0x020000, CRC(edbf2212) SHA1(a467baae065d0b7f19d32add1af6db226d1d2d85) )
	ROM_LOAD16_BYTE( "7614.bin", 0x000001, 0x020000, CRC(f3bc7c8b) SHA1(a46928b9c5c7c14c5c555fb893a528763cd35963) )
	j6oxo_sound
ROM_END

ROM_START( j6oxoc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7617.bin", 0x000000, 0x020000, CRC(25d4b1c2) SHA1(58526fd300882124cedc514bb6968729aee6a04b) )
	ROM_LOAD16_BYTE( "7618.bin", 0x000001, 0x020000, CRC(c88bdd77) SHA1(9c18c7c30e4d5c43752864eb4972a6d02865293f) )
	j6oxo_sound
ROM_END

ROM_START( j6oxod )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7619.bin", 0x000000, 0x020000, CRC(771071a3) SHA1(02c7490230b0f92cc6672934e5cc8d0fc21dba33) )
	ROM_LOAD16_BYTE( "7618.bin", 0x000001, 0x020000, CRC(c88bdd77) SHA1(9c18c7c30e4d5c43752864eb4972a6d02865293f) )
	j6oxo_sound
ROM_END

ROM_START( j6oxoe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7620.bin", 0x000000, 0x020000, CRC(ce4ae5fa) SHA1(fe46e4d3db6e5bcfd9b478e5ae5da035a677b941) )
	ROM_LOAD16_BYTE( "7618.bin", 0x000001, 0x020000, CRC(c88bdd77) SHA1(9c18c7c30e4d5c43752864eb4972a6d02865293f) )
	j6oxo_sound
ROM_END


#define j6oxobin_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "oxo_bingo_snd.bin", 0x000000, 0x080000, CRC(008a2d6a) SHA1(a89114154489142556b373ab24cd32fadf5856b3) )

ROM_START( j6oxobin )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7423.bin", 0x000000, 0x020000, CRC(f84de21b) SHA1(fb401153681ec271e8ddad4cd292a8c1dccfcb19) )
	ROM_LOAD16_BYTE( "7424.bin", 0x000001, 0x020000, CRC(7a4b827b) SHA1(28bd2e527c54780f2d866baa8909c1ade39825f6) )
	j6oxobin_sound
ROM_END

ROM_START( j6oxobina )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7425.bin", 0x000000, 0x020000, CRC(13d3b623) SHA1(bfc27e3ad60209f5567f2fa66cffb6a7a099bbce) )
	ROM_LOAD16_BYTE( "7424.bin", 0x000001, 0x020000, CRC(7a4b827b) SHA1(28bd2e527c54780f2d866baa8909c1ade39825f6) )
	j6oxobin_sound
ROM_END

ROM_START( j6oxobinb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7775.bin", 0x000000, 0x020000, CRC(ea87746c) SHA1(2608c4461362dc96527c04454b3f616b88a4dace) )
	ROM_LOAD16_BYTE( "7776.bin", 0x000001, 0x020000, CRC(a570d3b3) SHA1(1ee001078a87774167784555f57a95c6713057c1) )
	j6oxobin_sound
ROM_END

ROM_START( j6oxobinc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7777.bin", 0x000000, 0x020000, CRC(01192054) SHA1(7768d318532c37c08f8b695deb200a3d92cca550) )
	ROM_LOAD16_BYTE( "7776.bin", 0x000001, 0x020000, CRC(a570d3b3) SHA1(1ee001078a87774167784555f57a95c6713057c1) )
	j6oxobin_sound
ROM_END

ROM_START( j6oxobind )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7967.bin", 0x000000, 0x020000, CRC(03a5cb12) SHA1(7da2bec83e8808b40c96af8ac4741dfa1d8f6c39) )
	ROM_LOAD16_BYTE( "7968.bin", 0x000001, 0x020000, CRC(ec5aeab7) SHA1(2df0fbe853e90a82f0c2ed0bc2a728b17b99062a) )
	j6oxobin_sound
ROM_END

ROM_START( j6oxobine )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7969.bin", 0x000000, 0x020000, CRC(61c8ba9a) SHA1(bf0e979aaa36c11dac950e06cb713bb8b2b4ccdb) )
	ROM_LOAD16_BYTE( "7968.bin", 0x000001, 0x020000, CRC(ec5aeab7) SHA1(2df0fbe853e90a82f0c2ed0bc2a728b17b99062a) )
	j6oxobin_sound
ROM_END

ROM_START( j6oxobinf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7970.bin", 0x000000, 0x020000, CRC(e83b9f2a) SHA1(c297ed9066487b547a605636fdd8d3c04cc726a2) )
	ROM_LOAD16_BYTE( "7968.bin", 0x000001, 0x020000, CRC(ec5aeab7) SHA1(2df0fbe853e90a82f0c2ed0bc2a728b17b99062a) )
	j6oxobin_sound
ROM_END



ROM_START( j6pacman )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pac_1.bin", 0x000000, 0x020000, CRC(23c1f010) SHA1(3e9cb8e22e700fa28e1fad6300bc70567322383f) )
	ROM_LOAD16_BYTE( "pac_2.bin", 0x000001, 0x020000, CRC(773f33db) SHA1(5f61139bcccdea0f61c064dd2085a9904fa796ce) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) // which sound rom is right?
	ROM_LOAD( "pacman plus snd 1 65e5.bin", 0x000000, 0x080000, CRC(f0130ebe) SHA1(fd623add85d2faa556561f5b5b74c7c17d4ba02a) )
	ROM_LOAD( "pacsndv5.bin", 0x000000, 0x080000, CRC(cdb73ef6) SHA1(d204b50981aa34271c795c95b92e48371801cdd4) )
ROM_END

#define j6papa_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "papasnd.bin", 0x000000, 0x080000, CRC(c2de3abc) SHA1(2885817e7d6b11c0a2b35507b5654902257db32c) )

ROM_START( j6papa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pa0_4.p1", 0x000000, 0x020000, CRC(fc811398) SHA1(bdf8c8ccb67a5349f5a75502b22325a9d293a229) )
	ROM_LOAD16_BYTE( "pa0_4.p2", 0x000001, 0x020000, CRC(47b1317f) SHA1(a0f20e6ed92e8d7a4d76281649b34d181aa9638c) )
	j6papa_sound
ROM_END


ROM_START( j6papaa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pa0_5.p1", 0x000000, 0x020000, CRC(21d0a639) SHA1(0fe3ca2cdb73c1610b7bb293a9cc267263cf9020) )
	ROM_LOAD16_BYTE( "pa0_5.p2", 0x000001, 0x020000, CRC(d78a0063) SHA1(fd24ea795929e95006ca11237678277ad03fffff) )
	j6papa_sound
ROM_END

ROM_START( j6papab )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pa0_7.p1", 0x000000, 0x020000, CRC(60c23acd) SHA1(ece601078fc06c2ddb9b15ee6bc2e13739839fbc) )
	ROM_LOAD16_BYTE( "pa0_7.p2", 0x000001, 0x020000, CRC(1abfb80d) SHA1(940811dbd342f1a1dd4de7b82a7ecf0560412ad4) )
	j6papa_sound
ROM_END

ROM_START( j6papac )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pa1_0.p1", 0x000000, 0x020000, CRC(27e8b437) SHA1(5d06915d3c757466f0dcd0c4215efb73a5bf326a) )
	ROM_LOAD16_BYTE( "pa1_0.p2", 0x000000, 0x020000, CRC(62523eb7) SHA1(9c5b4a3729c154f64c907b731cb423a987e40f52) )
	j6papa_sound
ROM_END

ROM_START( j6papad )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pa1_1.p1", 0x000000, 0x020000, CRC(a1a88705) SHA1(b9d52bff66845e510e8236ab98630271121f0bbc) )
	ROM_LOAD16_BYTE( "pa1_1.p2", 0x000000, 0x020000, CRC(3aac53d3) SHA1(55f36dbd7e6d4353aae6b988f102392131ec9452) )
	j6papa_sound
ROM_END

ROM_START( j6papae )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "papa1_3.p1", 0x000000, 0x020000, CRC(5952b2ab) SHA1(3c69fd95d3470d8cbac22af91a21dc9f9e56e69d) )
	ROM_LOAD16_BYTE( "papa1_3.p2", 0x000000, 0x020000, CRC(80bd82a8) SHA1(abb497fecc37105d1436b77749137c7c6538a165) )
	j6papa_sound
ROM_END

ROM_START( j6papaf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "papa.p1", 0x000000, 0x020000, CRC(cb57a63c) SHA1(3472df9e375c820f1f8016528736bbfc37f6def3) )
	ROM_LOAD16_BYTE( "papa.p2", 0x000000, 0x020000, CRC(2ec81ffe) SHA1(9d8cb56a54576ec2f3b7a13793bdb4e6b5a765ca) )
	j6papa_sound
ROM_END

#define j6phxgld_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "pgsnd2.b8", 0x000000, 0x080000, CRC(c63cf006) SHA1(f204da5e744dd2ade662ac8d9f7d1896513cb38a) )
ROM_START( j6phxgld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pg30cz05_1.b8", 0x000000, 0x020000, CRC(9eb4716a) SHA1(459db543d20e5ddd03d0be917d8d4e153cb97183) )
	ROM_LOAD16_BYTE( "pg30cz05_2.b8", 0x000001, 0x020000, CRC(36003add) SHA1(f806358ee1111c3c57b90b50f6db5935d0aa26a6) )
	j6phxgld_sound
ROM_END

ROM_START( j6phxglda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pg75cz05_1.b8", 0x000000, 0x020000, CRC(92f9df5b) SHA1(2f817e065d0beb3c7a7acd0f4e4457f3ab4a80ad) )
	ROM_LOAD16_BYTE( "pg30cz05_2.b8", 0x000001, 0x020000, CRC(36003add) SHA1(f806358ee1111c3c57b90b50f6db5935d0aa26a6) )
	j6phxgld_sound
ROM_END

ROM_START( j6phxgldb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pg30cz07_1.b8", 0x000000, 0x020000, CRC(fe4270a9) SHA1(d8cbcc609c0a3af3de360504bc70757c981e4296) )
	ROM_LOAD16_BYTE( "pg30cz07_2.b8", 0x000001, 0x020000, CRC(0d4e4744) SHA1(9f1599859670b8973455ba6780ec4e2064ce29c2) )
	j6phxgld_sound
ROM_END

ROM_START( j6phxgldc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pg75cz07_1.b8", 0x000000, 0x020000, CRC(f20fde98) SHA1(a3f156031ef608a22111d4311b3be8aec4423ba0) )
	ROM_LOAD16_BYTE( "pg30cz07_2.b8", 0x000001, 0x020000, CRC(0d4e4744) SHA1(9f1599859670b8973455ba6780ec4e2064ce29c2) )
	j6phxgld_sound
ROM_END

ROM_START( j6phxgldd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pgcz30_04_1.b8", 0x000000, 0x020000, CRC(94724991) SHA1(bac5bf431dd46cbf1d177ebecd0b51e043d4764a) )
	ROM_LOAD16_BYTE( "pgcz30_04_2.b8", 0x000001, 0x020000, CRC(8a4f924c) SHA1(50ab5f41c72162f64a575a7143496a5ab12bdaa0) )
	j6phxgld_sound
ROM_END

ROM_START( j6phxglde )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pgcz75_04_1.b8", 0x000000, 0x020000, CRC(983fe7a0) SHA1(ef078fe2b17c8214ff58181f52c13de78c6bee9b) )
	ROM_LOAD16_BYTE( "pgcz30_04_2.b8", 0x000001, 0x020000, CRC(8a4f924c) SHA1(50ab5f41c72162f64a575a7143496a5ab12bdaa0) )
	j6phxgld_sound
ROM_END

ROM_START( j6phxgldf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pg30sl_02_1.b8", 0x000000, 0x020000, CRC(0fc9de3d) SHA1(590f581f144344947b17ff05345f46ce240572bb) )
	ROM_LOAD16_BYTE( "pg30sl_02_2.b8", 0x000001, 0x020000, CRC(85702a38) SHA1(014ed2f9c25f6f9d35288cbe99b2cab1fac3569b) )
	j6phxgld_sound
ROM_END

ROM_START( j6phxgldg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pg75cz1_1.b8", 0x000000, 0x020000, CRC(8f2b8818) SHA1(f4d1247f6b42741513dc698a19ff48032a05b8fb) )
	ROM_LOAD16_BYTE( "pg75cz1_2.b8", 0x000001, 0x020000, CRC(a6cacbd9) SHA1(bc253ce246d4fe6396efc1f163032e288da4bfec) )
	j6phxgld_sound
ROM_END


ROM_START( j6pnxgd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pccz2_1.b8", 0x000000, 0x020000, CRC(7d801a34) SHA1(ca5b6685d92c2d5489ea27af2044c1f9d7bd365c) )
	ROM_LOAD16_BYTE( "pccz2_2.b8", 0x000001, 0x020000, CRC(de2bd9ae) SHA1(cd64e18a5b9c3bef9589015b85b1abcc41aaed45) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "pgsnd2.b8", 0x000000, 0x080000, CRC(c63cf006) SHA1(f204da5e744dd2ade662ac8d9f7d1896513cb38a) )
ROM_END

#define j6pnxmil_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "phmill7_snd2", 0x000000, 0x014008, CRC(12086987) SHA1(ca6d74b844a0c042d66940f5d39fdef9d5591651) ) /* bad? */ \
	ROM_LOAD( "phmill7_snd.bin", 0x000000, 0x080000, CRC(e7332f6d) SHA1(c2457be9a7a37184bacd4199a5c347896ecfeb1c) )

ROM_START( j6pnxmil )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "phoenixmill_93001.b8", 0x000000, 0x020000, CRC(a7889afc) SHA1(800570f97be625e7fb1067e2f85c252bfc66c796) )
	ROM_LOAD16_BYTE( "phoenixmill_93002.b8", 0x000001, 0x020000, CRC(533fe752) SHA1(912ae6eeb9ccf9f2ae829816350eb3913bfe485e) ) // aka phoenixmill_97502.b8
	j6pnxmil_sound
ROM_END

ROM_START( j6pnxmila )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "phoenixmill_97501.b8", 0x000000, 0x020000, CRC(abc534cd) SHA1(413dc4781d8770f5e9162a33e3656f75f38de142) )
	ROM_LOAD16_BYTE( "phoenixmill_93002.b8", 0x000001, 0x020000, CRC(533fe752) SHA1(912ae6eeb9ccf9f2ae829816350eb3913bfe485e) ) // aka phoenixmill_97502.b8
	j6pnxmil_sound
ROM_END

ROM_START( j6pnxmilb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pm30cz10_1.b8", 0x000000, 0x020000, CRC(fc475584) SHA1(bfb96597c5d648aea67253f9f12ced0434680931) )
	ROM_LOAD16_BYTE( "pm30cz10_2.b8", 0x000001, 0x020000, CRC(cb87457b) SHA1(1f7578f07bc9e05d370390f46998c0036985b328) )
	j6pnxmil_sound
ROM_END

ROM_START( j6pnxmilc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pm75cz10_1.b8", 0x000000, 0x020000, CRC(f00afbb5) SHA1(e73c5daed0692be444be9ff143c7bff5bb8089d0) )
	ROM_LOAD16_BYTE( "pm30cz10_2.b8", 0x000001, 0x020000, CRC(cb87457b) SHA1(1f7578f07bc9e05d370390f46998c0036985b328) )
	j6pnxmil_sound
ROM_END

#define j6pinwzd_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "pbwzsnd.bin", 0x000000, 0x020000, CRC(ee9df577) SHA1(bdc6ecba5b7ad9c7b012342c7710266ec6eeb0ab) ) \
	ROM_LOAD( "piwi-snd.bin", 0x000000, 0x080000, CRC(ba98eecc) SHA1(39edb9524c23a78f89077215bef8f43a47605b47) )

ROM_START( j6pinwzd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "piwi-4s1.bin", 0x000000, 0x020000, CRC(7a0c9ef2) SHA1(2f375cce448c97a3f5905c1c8110e0bd39051842) )
	ROM_LOAD16_BYTE( "piwi-4s2.bin", 0x000001, 0x020000, CRC(3b270013) SHA1(f2570d66210fe37810afb5417217b0f50048af76) )
	j6pinwzd_sound
ROM_END

ROM_START( j6pinwzda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "piwi-4a1.bin", 0x000000, 0x020000, CRC(28c85e93) SHA1(3e4437cb2719b0648c8c6769cc867da836e3906b) )
	ROM_LOAD16_BYTE( "piwi-4s2.bin", 0x000001, 0x020000, CRC(3b270013) SHA1(f2570d66210fe37810afb5417217b0f50048af76) )
	j6pinwzd_sound
ROM_END

ROM_START( j6pinwzdb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "piwi-4p1.bin", 0x000000, 0x020000, CRC(9192caca) SHA1(333b2c75044aa846c4faeed1907aeb0dba52e7ab) )
	ROM_LOAD16_BYTE( "piwi-4s2.bin", 0x000001, 0x020000, CRC(3b270013) SHA1(f2570d66210fe37810afb5417217b0f50048af76) )
	j6pinwzd_sound
ROM_END

ROM_START( j6pinwzdc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "piwi-4wp.bin", 0x000000, 0x020000, CRC(f3ffbb42) SHA1(2ce69db92c408f4b43baa021a37a326e509c5c38) )
	ROM_LOAD16_BYTE( "piwi-4s2.bin", 0x000001, 0x020000, CRC(3b270013) SHA1(f2570d66210fe37810afb5417217b0f50048af76) )
	j6pinwzd_sound
ROM_END


ROM_START( j6pinwzdd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "piwi3p1", 0x000000, 0x020000, CRC(551c4596) SHA1(0a8aa7a0c17f4fb0e5e097677ec16e9f29b9f9e8) )
	ROM_LOAD16_BYTE( "piwi3p2", 0x000001, 0x020000, CRC(4dfcfdd5) SHA1(4e89e8d07771caaaa2f0c28c74db25af37ae634c) )
	j6pinwzd_sound
ROM_END

ROM_START( j6pinwzde )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pwiz15p1", 0x000000, 0x020000, CRC(ea4878e9) SHA1(0f61b9a971a9dfdb8cee9779d3e5b7dc705bba77) )
	ROM_LOAD16_BYTE( "pwiz15p2", 0x000001, 0x020000, CRC(561ee3df) SHA1(cfae78c0a9f448191aed16406fc46da208f5b62b) )
	j6pinwzd_sound
ROM_END

#define j6pinwzd_crystal_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6pinwzc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "pwc1.b8", 0x00000, 0x020000, CRC(f6099987) SHA1(0aabc7ba43b9144cf4a15354c4a9a17d54ebec0d) )
	ROM_LOAD16_BYTE(  "pwc2.b8", 0x00001, 0x020000, CRC(ea96ce13) SHA1(8897eb2decd2521dda2194a3be25d2b03e286d94) )
	j6pinwzd_crystal_sound
ROM_END

ROM_START( j6pinwzca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "pwd1.b8", 0x00000, 0x020000, CRC(b55c9ba9) SHA1(f53d68079378229f2311b190f8eda4b35b1d9913) )
	ROM_LOAD16_BYTE(  "pwd2.b8", 0x00001, 0x020000, CRC(ffe32710) SHA1(33ab7c95575587d036b3fc94d1f7363164748a97) )
	j6pinwzd_crystal_sound
ROM_END

ROM_START( j6pinwzcb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "pwp.b8", 0x00000, 0x020000, CRC(5ec2cf91) SHA1(0d8a63c884a33f1d46e27e2ce5b1a77b140b4062) )
	ROM_LOAD16_BYTE(  "pwd2.b8", 0x00001, 0x020000, CRC(ffe32710) SHA1(33ab7c95575587d036b3fc94d1f7363164748a97) )
	j6pinwzd_crystal_sound
ROM_END


#define j6pirgld_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "pirat6_snd.bin", 0x000000, 0x080000, CRC(5c60a3f8) SHA1(9d83aca9e5ecd230f6ca98f033f5274dbefe9feb) )
ROM_START( j6pirgld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pirat_113001.b8", 0x000000, 0x020000, CRC(58c488b8) SHA1(bd2cf3f604a1fd075d2077c8e38d75c0c3325cb8) )
	ROM_LOAD16_BYTE( "pirat_113002.b8", 0x000001, 0x020000, CRC(d38c52dd) SHA1(64f61a50c164d0d592bc566104a57a9630a78757) ) // aka pirat_117502.b8
	j6pirgld_sound
ROM_END

ROM_START( j6pirglda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pirat_117501.b8", 0x000000, 0x020000, CRC(54892689) SHA1(9b87c38e89db690971142066113b78a97b34719f) )
	ROM_LOAD16_BYTE( "pirat_113002.b8", 0x000001, 0x020000, CRC(d38c52dd) SHA1(64f61a50c164d0d592bc566104a57a9630a78757) ) // aka pirat_117502.b8
	j6pirgld_sound
ROM_END

#define j6popoli_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "popoli_snd.bin", 0x000000, 0x080000, CRC(e982a788) SHA1(3c6aed29892011c3d7d076ea9bef5882f7ded055) )
ROM_START( j6popoli )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7604.bin", 0x000000, 0x020000, CRC(7b44b69e) SHA1(8bbc3caa889d539646198c9b7f54cd31ab715c6d) )
	ROM_LOAD16_BYTE( "7605.bin", 0x000001, 0x020000, CRC(8b3cd10b) SHA1(92acf729c3bff02517f149dd80b18747d647bd2f) )
	j6popoli_sound
ROM_END

ROM_START( j6popolia )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7606.bin", 0x000000, 0x020000, CRC(90dae2a6) SHA1(38b790a3eb2aad3d2c039b27c18a1331b5c57b46) )
	ROM_LOAD16_BYTE( "7605.bin", 0x000001, 0x020000, CRC(8b3cd10b) SHA1(92acf729c3bff02517f149dd80b18747d647bd2f) )
	j6popoli_sound
ROM_END

#define j6pog_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "pog10snd.bin", 0x000000, 0x080000, CRC(00f6d1f6) SHA1(66581a6391e9ddc931cb102b00f38720ab125f5c) )

ROM_START( j6pog )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "potofgold1.bin", 0x000000, 0x020000, CRC(302e901e) SHA1(e88ded26da8b62b771eda0800e6e4afb1ae95ecf) )
	ROM_LOAD16_BYTE( "potofgold2.bin", 0x000001, 0x020000, CRC(40584378) SHA1(eeda580d65226feb642c541d1f16f2ff7b909098) )

	ROM_REGION( 0x1000000, "unkvideo", 0 )
	// these look like the roms in the 'impact video system' set? is this a video game? or are these misplaced / bad?
	// they do NOT belong with either snlad or tqst
	ROM_LOAD( "pog_1.bin", 0x000000, 0x100000, CRC(2244020f) SHA1(e392605473e5d0ff7d00bfe48d275d78a3417ded) )
	ROM_LOAD( "pog_2.bin", 0x000000, 0x100000, CRC(165f1742) SHA1(6b7b992dfa0383a50b67c4528d8a461149a65d2b) )

	j6pog_sound
ROM_END


ROM_START( j6poga )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9417.bin", 0x000000, 0x020000, CRC(89740447) SHA1(8528bf5faac53a375cd34cc329439a5bf6029ecd) )
	ROM_LOAD16_BYTE( "potofgold2.bin", 0x000001, 0x020000, CRC(40584378) SHA1(eeda580d65226feb642c541d1f16f2ff7b909098) )
	j6pog_sound
ROM_END

ROM_START( j6pogb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9419.bin", 0x000000, 0x020000, CRC(008721f7) SHA1(e438dd70b3018898ad13d0888ce67b0bbb3ca526) )
	ROM_LOAD16_BYTE( "potofgold2.bin", 0x000001, 0x020000, CRC(40584378) SHA1(eeda580d65226feb642c541d1f16f2ff7b909098) )
	j6pog_sound
ROM_END

ROM_START( j6pogc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pog5.3p1",       0x000000, 0x020000, CRC(62ea507f) SHA1(ea1e2057f3b51ab3f13c7b91467ed3142c64b76a) )
	ROM_LOAD16_BYTE( "potofgold2.bin", 0x000001, 0x020000, CRC(40584378) SHA1(eeda580d65226feb642c541d1f16f2ff7b909098) )
	j6pog_sound
ROM_END

ROM_START( j6pogd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ace pog v3a1,27c010", 0x000000, 0x020000, CRC(5b37fafc) SHA1(0aa7d0d813832f99150521d97ed754fa1f5f5fc6) )
	ROM_LOAD16_BYTE( "ace pog v3-2,27c010", 0x000001, 0x020000, CRC(28a2a74c) SHA1(c95a6a0a89cf942ba2460c46185e782f0445df2f) )
	j6pog_sound
ROM_END

ROM_START( j6potg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fxd.p1", 0x00000, 0x020000, CRC(113ddbe2) SHA1(492e320c296604ebd7bd5a55356698bfa36c1d0d) )
	ROM_LOAD16_BYTE(  "fxd.p2", 0x00001, 0x020000, CRC(c31eea57) SHA1(9b2a5cccbc7254c9a6b51f7ba5cc43c06578ed6e) )
	j6pog_sound
ROM_END

ROM_START( j6potga )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fxd_a.p1", 0x0000, 0x020000, CRC(43f91b83) SHA1(eaa1c1aa414ff378a7cb6820cfce7cc88fbfc559) )
	ROM_LOAD16_BYTE(  "fxd.p2", 0x00001, 0x020000, CRC(c31eea57) SHA1(9b2a5cccbc7254c9a6b51f7ba5cc43c06578ed6e) )
	j6pog_sound
ROM_END

ROM_START( j6potgb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fxd_p.p1", 0x0000, 0x020000, CRC(faa38fda) SHA1(437c1a34a044d2d4e17d1e94e145f3733d15bcf0) )
	ROM_LOAD16_BYTE(  "fxd.p2", 0x00001, 0x020000, CRC(c31eea57) SHA1(9b2a5cccbc7254c9a6b51f7ba5cc43c06578ed6e) )
	j6pog_sound
ROM_END




#define j6pogcls_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */
ROM_START( j6pogcls )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clpg-31.bin", 0x000000, 0x020000, CRC(07102281) SHA1(a7760c78f4848d7eec175027485d19d38307ccf9) )
	ROM_LOAD16_BYTE( "clpg-32.bin", 0x000001, 0x020000, CRC(8296488c) SHA1(8d3893fae9f2dde72a18e4b0a980814e3a3679ad) )
	j6pogcls_sound
ROM_END

ROM_START( j6pogclsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clpg-3a1.bin", 0x000000, 0x020000, CRC(55d4e2e0) SHA1(3d6d87c64392e863fc52a994c81a6f813cb06dfc) )
	ROM_LOAD16_BYTE( "clpg-32.bin", 0x000001, 0x020000, CRC(8296488c) SHA1(8d3893fae9f2dde72a18e4b0a980814e3a3679ad) )
	j6pogcls_sound
ROM_END

ROM_START( j6pogclsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clpg-3p1.bin", 0x000000, 0x020000, CRC(ec8e76b9) SHA1(53762b6d5f3d406b10bc5f9829826080cc590f40) )
	ROM_LOAD16_BYTE( "clpg-32.bin", 0x000001, 0x020000, CRC(8296488c) SHA1(8d3893fae9f2dde72a18e4b0a980814e3a3679ad) )
	j6pogcls_sound
ROM_END


#define j6pwrlin_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6pwrlin )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "powerl_63001.b8", 0x000000, 0x020000, CRC(28e5d1f3) SHA1(3acd26d88b19c61fce3286111d051dd0aaccd064) )
	ROM_LOAD16_BYTE( "powerl_63002.b8", 0x000001, 0x020000, CRC(789b3389) SHA1(4d497084489472da598d8d01811000a5ce14e0e8) ) // aka powerl_67502.b8
	j6pwrlin_sound
ROM_END

ROM_START( j6pwrlina )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "powerl_67501.b8", 0x000000, 0x020000, CRC(24a87fc2) SHA1(3a80a7aa2defe0d419ba619c8f0e9502556fbfd1) )
	ROM_LOAD16_BYTE( "powerl_63002.b8", 0x000001, 0x020000, CRC(789b3389) SHA1(4d497084489472da598d8d01811000a5ce14e0e8) ) // aka powerl_67502.b8
	j6pwrlin_sound
ROM_END

#define j6pwrspn_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "posp-snd.bin", 0x000000, 0x080000, CRC(861a0d14) SHA1(ea5eef793ad682dbf660ed7e77f93a7b900c97cc) )
ROM_START( j6pwrspn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "posp-5s1.bin", 0x000000, 0x020000, CRC(4184881b) SHA1(fb4dfb2e5b5c2cebd15b908a38014b56bb311eef) )
	ROM_LOAD16_BYTE( "posp-5s2.bin", 0x000001, 0x020000, CRC(7f888b32) SHA1(fed82966d74e9f8e0195b39a1ae267bff7c96677) )
	j6pwrspn_sound
ROM_END

ROM_START( j6pwrspna )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "posp-5a1.bin", 0x000000, 0x020000, CRC(1340487a) SHA1(c9f8c08ff4679f6a21dbb00d50f58e953a14716b) )
	ROM_LOAD16_BYTE( "posp-5s2.bin", 0x000001, 0x020000, CRC(7f888b32) SHA1(fed82966d74e9f8e0195b39a1ae267bff7c96677) )
	j6pwrspn_sound
ROM_END

ROM_START( j6pwrspnb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "posp-5p1.bin", 0x000000, 0x020000, CRC(aa1adc23) SHA1(a39b42de2ca2832e3c709f8b55bb15acc214bb73) )
	ROM_LOAD16_BYTE( "posp-5s2.bin", 0x000001, 0x020000, CRC(7f888b32) SHA1(fed82966d74e9f8e0195b39a1ae267bff7c96677) )
	j6pwrspn_sound
ROM_END

ROM_START( j6pwrspnc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "posp-5wp.bin", 0x000000, 0x020000, CRC(c877adab) SHA1(c4dcdeafd5c9d600f88e2b8d43c14bfcf7c0bbdd) )
	ROM_LOAD16_BYTE( "posp-5s2.bin", 0x000001, 0x020000, CRC(7f888b32) SHA1(fed82966d74e9f8e0195b39a1ae267bff7c96677) )
	j6pwrspn_sound
ROM_END

ROM_START( j6pwrspnd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "posp4p1", 0x000000, 0x020000, CRC(798af2ba) SHA1(912e6307a39239419b9e9295706e070632168ce0) )
	ROM_LOAD16_BYTE( "posp4p2", 0x000001, 0x020000, CRC(25883a02) SHA1(a5eb7c27e3e72e5609ee4c98a57e552f8feabffe) )
	j6pwrspn_sound
ROM_END

ROM_START( j6pwrspne )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pows15p1", 0x000000, 0x020000, CRC(a6d11f57) SHA1(489cd1cdd505ca4c9db87b0b8baf1cb0d43646ba) )
	ROM_LOAD16_BYTE( "pows15p2", 0x000001, 0x020000, CRC(e6f0e76d) SHA1(8138991e102f86b7f984c61a24f9255e726c807e) )
	j6pwrspn_sound
ROM_END

#define j6quantm_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "qule-snd.bin", 0x000000, 0x080000, CRC(eb8c692d) SHA1(384b73573d64d67547d1c04f279bda6c02f78450) )
ROM_START( j6quantm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20717.bin", 0x000000, 0x020000, CRC(31435fe1) SHA1(d42fe30367ded93562d4e1739307a47423f4dd51) )
	ROM_LOAD16_BYTE( "20718.bin", 0x000001, 0x020000, CRC(4d6f4b1f) SHA1(f562c677cf920fe2a0e5edec2e4f241855e005c3) )
	j6quantm_sound
ROM_END

ROM_START( j6quantma )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20719.bin", 0x000000, 0x020000, CRC(dadd0bd9) SHA1(6bbe782b46ed5fe0677845548077593df25d1c0d) )
	ROM_LOAD16_BYTE( "20718.bin", 0x000001, 0x020000, CRC(4d6f4b1f) SHA1(f562c677cf920fe2a0e5edec2e4f241855e005c3) )
	j6quantm_sound
ROM_END

ROM_START( j6quantmb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20720.bin", 0x000000, 0x020000, CRC(63879f80) SHA1(3c1b818378c8995fdadbec0aeda2d2b04db89e6a) )
	ROM_LOAD16_BYTE( "20718.bin", 0x000001, 0x020000, CRC(4d6f4b1f) SHA1(f562c677cf920fe2a0e5edec2e4f241855e005c3) )
	j6quantm_sound
ROM_END

ROM_START( j6quantmc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20721.bin", 0x000000, 0x020000, CRC(532e2e69) SHA1(70661489c4c16af0a59fcfc3f68e4182aa14a8be) )
	ROM_LOAD16_BYTE( "20718.bin", 0x000001, 0x020000, CRC(4d6f4b1f) SHA1(f562c677cf920fe2a0e5edec2e4f241855e005c3) )
	j6quantm_sound
ROM_END



ROM_START( j6quick )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "quicksilver_a1.bin", 0x000000, 0x020000, CRC(ddca1835) SHA1(adb8a9320c8ab2c5c6d8ec83b1ed577a8eb4ba91) )
	ROM_LOAD16_BYTE( "quicksilver_a2.bin", 0x000001, 0x020000, CRC(efa8765d) SHA1(c1174f9ab8f687d1dd8b4d50ff519550a0643219) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "quicksilver_snd.bin", 0x000000, 0x080000, CRC(374d8892) SHA1(38918bbcb7d9117b21b6dafdb55cc5c36927fb4e) )
ROM_END


ROM_START( j6rager )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rag.p1", 0x000000, 0x020000, CRC(d8f3a090) SHA1(17b5bbf09d5a4f31d0fe4f6561fb03e97e3e4c9a) )
	ROM_LOAD16_BYTE( "rag.p2", 0x000001, 0x020000, CRC(5d176dc3) SHA1(c2641f01d57fd2eb7247252cc42a92e21e0f60a5) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "ragsnd.bin", 0x000000, 0x080000, CRC(5eadea62) SHA1(f1f6800a6479ae49a4774ee597e3d58e036f4100) )
ROM_END

#define j6ra_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "realsnd.bin", 0x000000, 0x080000, CRC(8bc92c90) SHA1(bcbbe270ce42d5960ac37a2324e3fb37ff513147) )

ROM_START( j6ra )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "real-5s1.bin", 0x000000, 0x020000, CRC(b1894f2e) SHA1(dc77a2b40e9fee9bdc81697bf27ec81e420b06ea) )
	ROM_LOAD16_BYTE( "real-5s2.bin", 0x000001, 0x020000, CRC(01b3214d) SHA1(6a16c2e5045e85bfe26d805a1157eab0f0aa0cb0) )
	j6ra_sound
ROM_END

ROM_START( j6raa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "real-5a1.bin", 0x000000, 0x020000, CRC(e34d8f4f) SHA1(4df257c8c8602035e6a174f4ed5698f2ef911021) )
	ROM_LOAD16_BYTE( "real-5s2.bin", 0x000001, 0x020000, CRC(01b3214d) SHA1(6a16c2e5045e85bfe26d805a1157eab0f0aa0cb0) )
	j6ra_sound
ROM_END

ROM_START( j6rab )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "real-5p1.bin", 0x000000, 0x020000, CRC(5a171b16) SHA1(e5ddd23f1d3a062cd6b57dffbf88edb043ae9a6e) )
	ROM_LOAD16_BYTE( "real-5s2.bin", 0x000001, 0x020000, CRC(01b3214d) SHA1(6a16c2e5045e85bfe26d805a1157eab0f0aa0cb0) )
	j6ra_sound
ROM_END

ROM_START( j6rac )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "real-5wp.bin", 0x000000, 0x020000, CRC(387a6a9e) SHA1(8f47ec2d58dfb85a27e2574f096ba3cc79036948) )
	ROM_LOAD16_BYTE( "real-5s2.bin", 0x000001, 0x020000, CRC(01b3214d) SHA1(6a16c2e5045e85bfe26d805a1157eab0f0aa0cb0) )
	j6ra_sound
ROM_END

ROM_START( j6rad )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "realr3p1", 0x000000, 0x020000, CRC(ad9d92d3) SHA1(874a37e9db59ce8dd83fb96e8ae0ec1bd64aa1ae) )
	ROM_LOAD16_BYTE( "realr3p2", 0x000001, 0x020000, CRC(37ac2694) SHA1(b603f84146bea70794e98dab47d705b180e72b8d) )
	j6ra_sound
ROM_END

ROM_START( j6redal )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "real-c1.bin", 0x00000, 0x020000, CRC(eabec7ae) SHA1(5722dc489b3cf5ff90d9688d52fd8489d80b9c96) )
	ROM_LOAD16_BYTE(  "real-c2.bin", 0x00001, 0x020000, CRC(104bbeee) SHA1(1f4a3d05c729e03f91b9fa09c0d09b952ea2ce9d) )
	j6ra_sound
ROM_END

ROM_START( j6redala )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "raa1a.bin", 0x0000, 0x020000, CRC(839e7773) SHA1(57dcd6fcf933ea3f2d903a62f673a1c71d028748) )
	ROM_LOAD16_BYTE(  "raa2.bin", 0x0001, 0x020000, CRC(bbfa6d4d) SHA1(2a1e1df72acb95e1e8eaa498727aafd1734bb5d1) )
	j6ra_sound
ROM_END



#define j6raclb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "realsnd.bin", 0x000000, 0x080000, CRC(8bc92c90) SHA1(bcbbe270ce42d5960ac37a2324e3fb37ff513147) )

ROM_START( j6raclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20953.bin", 0x000000, 0x020000, CRC(ed7afbad) SHA1(fdb2af1dedfaf1a5dfa543ba58bf4420b19c3454) )
	ROM_LOAD16_BYTE( "20950.bin", 0x000001, 0x020000, CRC(5c4659d9) SHA1(aa69f2c1d4ea6755ef1ed7e3d040598befcb6690) )
	j6raclb_sound
ROM_END

ROM_START( j6raclba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "racl-fs1.bin", 0x000000, 0x020000, CRC(b361ab58) SHA1(129feeadc9f0026fa86c39f42e506027b6665dfa) )
	ROM_LOAD16_BYTE( "racl-fs2.bin", 0x000001, 0x020000, CRC(a1ad549a) SHA1(e33184262c9e76e8fecdcec9fa274baa16ba9d67) )
	j6raclb_sound
ROM_END

ROM_START( j6raclbb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "racl-ff1.bin", 0x000000, 0x020000, CRC(77bb4848) SHA1(4900a09cf0712d87aacf4772406a4d284c6e7d9e) )
	ROM_LOAD16_BYTE( "racl-fs2.bin", 0x000001, 0x020000, CRC(a1ad549a) SHA1(e33184262c9e76e8fecdcec9fa274baa16ba9d67) )
	j6raclb_sound
ROM_END

ROM_START( j6raclbc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "racl-fi1.bin", 0x000000, 0x020000, CRC(e1a56b39) SHA1(036850fcfd39b1194718c7bf575991514015d015) )
	ROM_LOAD16_BYTE( "racl-fs2.bin", 0x000001, 0x020000, CRC(a1ad549a) SHA1(e33184262c9e76e8fecdcec9fa274baa16ba9d67) )
	j6raclb_sound
ROM_END

ROM_START( j6raclbd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "racl-fp1.bin", 0x000000, 0x020000, CRC(58ffff60) SHA1(e68d9b8d8523732ced12eb7bfb8a749499ea01c1) )
	ROM_LOAD16_BYTE( "racl-fs2.bin", 0x000001, 0x020000, CRC(a1ad549a) SHA1(e33184262c9e76e8fecdcec9fa274baa16ba9d67) )
	j6raclb_sound
ROM_END


#define j6redarw_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "rear-snd.bin", 0x000000, 0x080000, CRC(7e8c05ce) SHA1(616b0f94b94331f86d7b1fec11dd05cf9b0968cf) )

ROM_START( j6redarw )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-6_1.bin", 0x000000, 0x020000, CRC(19b81492) SHA1(2714248444c9dc800eb8cfed67106b33e1e070e3) )
	ROM_LOAD16_BYTE( "rear-6_2.bin", 0x000001, 0x020000, CRC(691cc832) SHA1(c68bee5abbec6d3b28030ee58991f0c4abe70d35) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-6a1.bin", 0x000000, 0x020000, CRC(4b7cd4f3) SHA1(1d8cb022b3d9ba07be6da0f8f068875386f6b73a) )
	ROM_LOAD16_BYTE( "rear-6_2.bin", 0x000001, 0x020000, CRC(691cc832) SHA1(c68bee5abbec6d3b28030ee58991f0c4abe70d35) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-6n1.bin", 0x000000, 0x020000, CRC(288eac56) SHA1(1fbfa85cdfbf4bf23d2e2989eabf16e2536e9ae3) )
	ROM_LOAD16_BYTE( "rear-6_2.bin", 0x000001, 0x020000, CRC(691cc832) SHA1(c68bee5abbec6d3b28030ee58991f0c4abe70d35) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-6np.bin", 0x000000, 0x020000, CRC(c310f86e) SHA1(28b2c2993e727dac1201323d89f3568704e1acdd) )
	ROM_LOAD16_BYTE( "rear-6_2.bin", 0x000001, 0x020000, CRC(691cc832) SHA1(c68bee5abbec6d3b28030ee58991f0c4abe70d35) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-6p1.bin", 0x000000, 0x020000, CRC(f22640aa) SHA1(72793eb730ea970815a95202e4c44ebb3f15c124) )
	ROM_LOAD16_BYTE( "rear-6_2.bin", 0x000001, 0x020000, CRC(691cc832) SHA1(c68bee5abbec6d3b28030ee58991f0c4abe70d35) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-6w1.bin", 0x000000, 0x020000, CRC(7bd5651a) SHA1(2d1caf1314bcbf95f2da98de0a6782a073cdbb4c) )
	ROM_LOAD16_BYTE( "rear-6_2.bin", 0x000001, 0x020000, CRC(691cc832) SHA1(c68bee5abbec6d3b28030ee58991f0c4abe70d35) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-7_1.bin", 0x000000, 0x020000, CRC(6d36f687) SHA1(d07a2058fc0f7a7f1cac6398c46f6fc2fb676484) )
	ROM_LOAD16_BYTE( "rear-7_2.bin", 0x000001, 0x020000, CRC(962c6cac) SHA1(dc5fb331df921ef7b94d2c0d3e7d0bb299e728a3) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-7a1.bin", 0x000000, 0x020000, CRC(3ff236e6) SHA1(f0c42885b6f908ffdcf41ae3b6789bfa95c8f36f) )
	ROM_LOAD16_BYTE( "rear-7_2.bin", 0x000001, 0x020000, CRC(962c6cac) SHA1(dc5fb331df921ef7b94d2c0d3e7d0bb299e728a3) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-7n1.bin", 0x000000, 0x020000, CRC(5c004e43) SHA1(eb4dfdb35efc5226df366cd3f2cdcd486098ba53) )
	ROM_LOAD16_BYTE( "rear-7_2.bin", 0x000001, 0x020000, CRC(962c6cac) SHA1(dc5fb331df921ef7b94d2c0d3e7d0bb299e728a3) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear-7p1.bin", 0x000000, 0x020000, CRC(86a8a2bf) SHA1(1c7301c3e5a3ff1d791b21e617d50875c06a7463) )
	ROM_LOAD16_BYTE( "rear-7_2.bin", 0x000001, 0x020000, CRC(962c6cac) SHA1(dc5fb331df921ef7b94d2c0d3e7d0bb299e728a3) )
	j6redarw_sound
ROM_END

ROM_START( j6redarwj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rear7np1.bin", 0x000000, 0x020000, CRC(b79e1a7b) SHA1(806ae7180dbee9b605bd8d923179a2323a7d38ee) )
	ROM_LOAD16_BYTE( "rear-7_2.bin", 0x000001, 0x020000, CRC(962c6cac) SHA1(dc5fb331df921ef7b94d2c0d3e7d0bb299e728a3) )
	j6redarw_sound
ROM_END

ROM_START( j6redarww )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	// invalid pairing?
	ROM_LOAD( "redarrow.p1", 0x000000, 0x020000, CRC(e47fdd2c) SHA1(1cbce6e38cacad4c0ec617e38522ef81feaeb296) )
	ROM_LOAD( "redarrow.p2", 0x000001, 0x020000, CRC(afccd6c4) SHA1(5cfcb7132a169ea13fc0b48fc2d34071243a9046) )
	j6redarw_sound
ROM_END



#define j6rh6_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6rh6 )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhsx-a1.bin", 0x000000, 0x020000, CRC(fa5d4bb2) SHA1(22c896babcc052214a87e928006826ab6e8795bf) )
	ROM_LOAD16_BYTE( "rhsx-a2.bin", 0x000001, 0x020000, CRC(7d3435f0) SHA1(7d2b55d6b40fe069123fea16a92a4db2490bebe8) )
	j6rh6_sound
ROM_END

ROM_START( j6rh6a )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhsx-f1.bin", 0x000000, 0x020000, CRC(781c2139) SHA1(87f594ce9285142fd8e6553a9e24ca4f224e4fef) )
	ROM_LOAD16_BYTE( "rhsx-f2.bin", 0x000001, 0x020000, CRC(a9319b7f) SHA1(e8c393868fdf26fd30741f936b5ea708ea0c3fc7) )
	j6rh6_sound
ROM_END

ROM_START( j6rh6b )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhsx-fb1.bin", 0x000000, 0x020000, CRC(74518f08) SHA1(9d63ba9730e17ab13f7eab0ea49d833553017701) )
	ROM_LOAD16_BYTE( "rhsx-f2.bin", 0x000001, 0x020000, CRC(a9319b7f) SHA1(e8c393868fdf26fd30741f936b5ea708ea0c3fc7) )
	j6rh6_sound
ROM_END

ROM_START( j6rh6c )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhsx-fe1.bin", 0x000000, 0x020000, CRC(60877d5b) SHA1(247421c1205dab12cb4e1701ecf667716dee7149) )
	ROM_LOAD16_BYTE( "rhsx-f2.bin", 0x000001, 0x020000, CRC(a9319b7f) SHA1(e8c393868fdf26fd30741f936b5ea708ea0c3fc7) )
	j6rh6_sound
ROM_END

ROM_START( j6rh6d )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhsx-ff1.bin", 0x000000, 0x020000, CRC(bcc6c229) SHA1(7775b137649b66904bf38e9d480a70b9d62292bb) )
	ROM_LOAD16_BYTE( "rhsx-f2.bin", 0x000001, 0x020000, CRC(a9319b7f) SHA1(e8c393868fdf26fd30741f936b5ea708ea0c3fc7) )
	j6rh6_sound
ROM_END

ROM_START( j6rh6e )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhsx-fp1.bin", 0x000000, 0x020000, CRC(93827501) SHA1(eac03990095f3ae2d00b6822b3cc8b0790f8507b) )
	ROM_LOAD16_BYTE( "rhsx-f2.bin", 0x000001, 0x020000, CRC(a9319b7f) SHA1(e8c393868fdf26fd30741f936b5ea708ea0c3fc7) )
	j6rh6_sound
ROM_END


#define j6rhchil_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "rhcssnd.bin", 0x000000, 0x080000, CRC(60b336b1) SHA1(53d04bec9cbba4a0e89d34329ed41f89945e283b) )
ROM_START( j6rhchil )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhcst.p1", 0x000000, 0x020000, CRC(334fdbcc) SHA1(194c226e1c41eb326841cf022e8a1b28088a7073) )
	ROM_LOAD16_BYTE( "rhcst.p2", 0x000001, 0x020000, CRC(e4a12747) SHA1(f6f0388dcf2713f4c289b4ae313cfccf1d308963) )
	j6rhchil_sound
ROM_END

ROM_START( j6rhchila )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhcs-3a1.bin", 0x000000, 0x020000, CRC(618b1bad) SHA1(3461d18d89694f4a28f1ae63dcea69a3413fef9d) )
	ROM_LOAD16_BYTE( "rhcst.p2", 0x000001, 0x020000, CRC(e4a12747) SHA1(f6f0388dcf2713f4c289b4ae313cfccf1d308963) )
	j6rhchil_sound
ROM_END

ROM_START( j6rhchilb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhcs-3l1.bin", 0x000000, 0x020000, CRC(f79538dc) SHA1(6f6986b949a36229eb4a61b3dbfeb60a3d51cc6d) )
	ROM_LOAD16_BYTE( "rhcst.p2", 0x000001, 0x020000, CRC(e4a12747) SHA1(f6f0388dcf2713f4c289b4ae313cfccf1d308963) )
	j6rhchil_sound
ROM_END

ROM_START( j6rhchilc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhcs-3p1.bin", 0x000000, 0x020000, CRC(d8d18ff4) SHA1(131caae565e480bf6d893591284b5f5f998deb32) )
	ROM_LOAD16_BYTE( "rhcst.p2", 0x000001, 0x020000, CRC(e4a12747) SHA1(f6f0388dcf2713f4c289b4ae313cfccf1d308963) )
	j6rhchil_sound
ROM_END

ROM_START( j6rhchild )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rhcs3lp1.bin", 0x000000, 0x020000, CRC(1c0b6ce4) SHA1(8713aae84f960d4fb121ce5552d1b14692b39c28) )
	ROM_LOAD16_BYTE( "rhcst.p2", 0x000001, 0x020000, CRC(e4a12747) SHA1(f6f0388dcf2713f4c289b4ae313cfccf1d308963) )
	j6rhchil_sound
ROM_END


#define j6rh6cl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "rh6s1.bin", 0x000000, 0x080000, CRC(86b4c970) SHA1(0ce214ee406b1c325693b2b615498bdb2c3a16eb) )

ROM_START( j6rh6cl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8edf", 0x000000, 0x020000, CRC(e0b6b3cb) SHA1(4c174f3a5ef0bd74877c2a769c60375cb34a3a6f) )
	ROM_LOAD16_BYTE( "f903.bin", 0x000001, 0x020000, CRC(ba024f9b) SHA1(f078d1ecbf8397f0cad99957c081acc44e40b5cb) )
	j6rh6cl_sound
ROM_END

ROM_START( j6rh6cla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8ef7", 0x000000, 0x020000, CRC(155ae81f) SHA1(f03d04bd6b3b5f90be10e137984dda42c9d53c17) )
	ROM_LOAD16_BYTE( "f903.bin", 0x000001, 0x020000, CRC(ba024f9b) SHA1(f078d1ecbf8397f0cad99957c081acc44e40b5cb) )
	j6rh6cl_sound
ROM_END

ROM_START( j6rh6clb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8efb", 0x000000, 0x020000, CRC(3cf70cb9) SHA1(a31041b35310a5a10845ac2330df677fe2f0b95e) )
	ROM_LOAD16_BYTE( "f903.bin", 0x000001, 0x020000, CRC(ba024f9b) SHA1(f078d1ecbf8397f0cad99957c081acc44e40b5cb) )
	j6rh6cl_sound
ROM_END

ROM_START( j6rh6clc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8efe", 0x000000, 0x020000, CRC(cff204e3) SHA1(7b7c1558ea491fbb1fe61eeb932ccbd0e14e266d) )
	ROM_LOAD16_BYTE( "f903.bin", 0x000001, 0x020000, CRC(ba024f9b) SHA1(f078d1ecbf8397f0cad99957c081acc44e40b5cb) )
	j6rh6cl_sound
ROM_END

ROM_START( j6rh6cld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8eff.bin", 0x000000, 0x020000, CRC(246c50db) SHA1(b210d611ed4d0e410ebe65236b4acca3b2e07b7f) )
	ROM_LOAD16_BYTE( "f903.bin", 0x000001, 0x020000, CRC(ba024f9b) SHA1(f078d1ecbf8397f0cad99957c081acc44e40b5cb) )
	j6rh6cl_sound
ROM_END


#define j6reelmn_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "1360.bin", 0x000000, 0x080000, CRC(3a1a5f09) SHA1(807cf2cf7a4738c1904990b281f7d4c9a86c78e7) )
ROM_START( j6reelmn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9485.bin", 0x000000, 0x020000, CRC(1a729666) SHA1(d4389ff5a16b994ca1480fe0ff097c7601b2ef6b) )
	ROM_LOAD16_BYTE( "9486.bin", 0x000001, 0x020000, CRC(522b3e21) SHA1(caef56b3479ebe0007a420cbe1c0766e121b05c1) )
	j6reelmn_sound
ROM_END

ROM_START( j6reelmna )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9487.bin", 0x000000, 0x020000, CRC(f1ecc25e) SHA1(a5240f0fae7ddc056bfc974e37fabf5d51d7db53) )
	ROM_LOAD16_BYTE( "9486.bin", 0x000001, 0x020000, CRC(522b3e21) SHA1(caef56b3479ebe0007a420cbe1c0766e121b05c1) )
	j6reelmn_sound
ROM_END

ROM_START( j6reelmnb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9488.bin", 0x000000, 0x020000, CRC(48b65607) SHA1(edeb98305907951c40a247a2f1337b77727c6d71) )
	ROM_LOAD16_BYTE( "9486.bin", 0x000001, 0x020000, CRC(522b3e21) SHA1(caef56b3479ebe0007a420cbe1c0766e121b05c1) )
	j6reelmn_sound
ROM_END

ROM_START( j6reelmnc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9489.bin", 0x000000, 0x020000, CRC(781fe7ee) SHA1(f7799b5f74916aab64538b3467d2363a754f8af5) )
	ROM_LOAD16_BYTE( "9486.bin", 0x000001, 0x020000, CRC(522b3e21) SHA1(caef56b3479ebe0007a420cbe1c0766e121b05c1) )
	j6reelmn_sound
ROM_END

ROM_START( j6reelmnd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "remov7p1", 0x000000, 0x020000, CRC(55a48785) SHA1(7cea4fb4e9d521d6512e2ae0da36ee243d29e998) )
	ROM_LOAD16_BYTE( "remov7p2", 0x000001, 0x020000, CRC(8fd6fefc) SHA1(91d6d2f0915a20e2177992b360af763bcfe30cf7) )
	j6reelmn_sound
ROM_END



ROM_START( j6reelth )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "reelthing.p1", 0x000000, 0x020000, CRC(74b90cef) SHA1(097fbbdd049a85e9a9251858fb80cf9cde2397fc) )
	ROM_LOAD16_BYTE( "reelthing.p2", 0x000001, 0x020000, CRC(c219443a) SHA1(742d9d7f8c1071c5860b34449909ca1d2decc053) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "reelthingsnd.bin", 0x000000, 0x080000, CRC(44d72e51) SHA1(f472e9effbd925ecc5db6dd47eddf3f5cea8fe46) )
ROM_END

#define j6reelth_alt_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "reel-snd.bin", 0x000000, 0x080000, CRC(3178ddb0) SHA1(60be12e6198bd8b7cf021b54c3cd858ff5bac127) )
ROM_START( j6reeltha )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "reel-31.bin", 0x000000, 0x020000, CRC(3e8020f8) SHA1(479f9e157eba570d0fa670f0f9ea7dbd3d94ca1f) )
	ROM_LOAD16_BYTE( "reel-32.bin", 0x000001, 0x020000, CRC(49b1cb81) SHA1(5f39416d4a74a7af36909bc2afc7a568957fc8c1) )
	j6reelth_alt_sound
ROM_END

ROM_START( j6reelthb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "reel-3p1.bin", 0x000000, 0x020000, CRC(d51e74c0) SHA1(d0f7c4c30a6943c7e429e57b16b9eef989b253ff) )
	ROM_LOAD16_BYTE( "reel-32.bin", 0x000001, 0x020000, CRC(49b1cb81) SHA1(5f39416d4a74a7af36909bc2afc7a568957fc8c1) )
	j6reelth_alt_sound
ROM_END



ROM_START( j6richpk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rich1p.bin", 0x000000, 0x020000, CRC(e99fd568) SHA1(ae71eb6a9871fa9856fb62d2a5158776d9c2bddf) )
	ROM_LOAD16_BYTE( "rich2.bin",  0x000001, 0x020000, CRC(39d5a254) SHA1(464696715769f1e15c80acf4116d7718490abf8c) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END


#define j6rico_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "ricochet_snd.bin", 0x000000, 0x080000, CRC(b24522fe) SHA1(1546edee6cf483abdbc761c715dcbc696209d429) )

ROM_START( j6rico )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9523.bin", 0x000000, 0x020000, CRC(4eee01c3) SHA1(f2336b4732efa0b86ee8a0df6d975355c2f27367) )
	ROM_LOAD16_BYTE( "9524.bin", 0x000001, 0x020000, CRC(7d17a2ec) SHA1(eba6e5cdee844bdce4d9657009c64433307e49af) )
	j6rico_sound
ROM_END

ROM_START( j6ricoa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9525.bin", 0x000000, 0x020000, CRC(a57055fb) SHA1(ed1572f8a0a40ea48a1bf4249a424780286f484a) )
	ROM_LOAD16_BYTE( "9524.bin", 0x000001, 0x020000, CRC(7d17a2ec) SHA1(eba6e5cdee844bdce4d9657009c64433307e49af) )
	j6rico_sound
ROM_END

ROM_START( j6ricob )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9526.bin", 0x000000, 0x020000, CRC(1c2ac1a2) SHA1(2aa10007b06bb5fb4e4220d99c5928bda3bb756f) )
	ROM_LOAD16_BYTE( "9524.bin", 0x000001, 0x020000, CRC(7d17a2ec) SHA1(eba6e5cdee844bdce4d9657009c64433307e49af) )
	j6rico_sound
ROM_END

ROM_START( j6ricoc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9527.bin", 0x000000, 0x020000, CRC(2c83704b) SHA1(34787e1c6664838476fd6f8085752e1647f3b765) )
	ROM_LOAD16_BYTE( "9524.bin", 0x000001, 0x020000, CRC(7d17a2ec) SHA1(eba6e5cdee844bdce4d9657009c64433307e49af) )
	j6rico_sound
ROM_END

ROM_START( j6ricod )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ricov5p1", 0x000000, 0x020000, CRC(3f020a40) SHA1(98b00f4e5720135e71176bb5507f194468fd99ef) )
	ROM_LOAD16_BYTE( "ricov5p2", 0x000001, 0x020000, CRC(7510a81d) SHA1(8c46008bd8e8fbd9439370383c378ea19e0963c8) )
	j6rico_sound
ROM_END

ROM_START( j6ricoe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rico-51a.p1", 0x000000, 0x020000, CRC(6dc6ca21) SHA1(f0e38d90e0613a899aba520f3ecc746195a4b8fe) )
	ROM_LOAD16_BYTE( "ricov5p2", 0x000001, 0x020000, CRC(7510a81d) SHA1(8c46008bd8e8fbd9439370383c378ea19e0963c8) )
	j6rico_sound
ROM_END


#define j6robin_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "robin_snd.bin", 0x000000, 0x080000, CRC(bbddccf9) SHA1(33f3d14d4898f9ba4ba2c2a88621cf3e2c828a8f) )
ROM_START( j6robin )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9425.bin", 0x000000, 0x020000, CRC(0b9f9c83) SHA1(2004b7fc3a0fd8ba424d0e293cd53b57a653f6e3) )
	ROM_LOAD16_BYTE( "9426.bin", 0x000001, 0x020000, CRC(a85c6c19) SHA1(eab10967642f801dcd25dcdd4dfe95ac3b3491bf) )
	j6robin_sound
ROM_END

ROM_START( j6robina )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9427.bin", 0x000000, 0x020000, CRC(e001c8bb) SHA1(084182d27ddd4146e75ec5cdc9c20e09f4bb390b) )
	ROM_LOAD16_BYTE( "9426.bin", 0x000001, 0x020000, CRC(a85c6c19) SHA1(eab10967642f801dcd25dcdd4dfe95ac3b3491bf) )
	j6robin_sound
ROM_END

ROM_START( j6robinb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9428.bin", 0x000000, 0x020000, CRC(595b5ce2) SHA1(276290e86454b2f1fc73b2cc3e545b6c60363535) )
	ROM_LOAD16_BYTE( "9426.bin", 0x000001, 0x020000, CRC(a85c6c19) SHA1(eab10967642f801dcd25dcdd4dfe95ac3b3491bf) )
	j6robin_sound
ROM_END

ROM_START( j6robinc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9429.bin", 0x000000, 0x020000, CRC(69f2ed0b) SHA1(dd759bd012f55fb94982c748f0cbc02178438abd) ) // aka robin_a_wb.bin
	ROM_LOAD16_BYTE( "9426.bin", 0x000001, 0x020000, CRC(a85c6c19) SHA1(eab10967642f801dcd25dcdd4dfe95ac3b3491bf) ) // aka robin_b_wb.bin
	j6robin_sound
ROM_END

#define j6roller_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "rocosnd.bin", 0x000000, 0x080000, CRC(60236e81) SHA1(9546c92d8a42d911e5b462c598a0b42987c0ba03) )

// this is the same as rocosnd.bin but with 0x77b50 as 0xf6 in the middle of an 0xff fill area, assume it to be bad
//ROM_LOAD( "rcstrsnd.bin", 0x000000, 0x080000, CRC(b0753c1d) SHA1(b111ca10c01ee2089cbc613ad91235d429272ab8) )
ROM_START( j6roller )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9460.bin", 0x000000, 0x020000, CRC(28d13bcb) SHA1(1103b06830cbcd25e856b99beb307f69f0fee6fd) )
	ROM_LOAD16_BYTE( "9461.bin", 0x000001, 0x020000, CRC(b7515266) SHA1(d0b4d9b1dbc968f289e04398e81b585e71e3b358) )
	j6roller_sound
ROM_END

ROM_START( j6rollera )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9462.bin", 0x000000, 0x020000, CRC(c34f6ff3) SHA1(fd93acc1b0f985b3fa4b822791a6c3864d8d8ad0) )
	ROM_LOAD16_BYTE( "9461.bin", 0x000001, 0x020000, CRC(b7515266) SHA1(d0b4d9b1dbc968f289e04398e81b585e71e3b358) )
	j6roller_sound
ROM_END

ROM_START( j6rollerb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9463.bin", 0x000000, 0x020000, CRC(7a15fbaa) SHA1(37fd6ddce0d2262df5ba32b90cf0fe664f9c7de1) )
	ROM_LOAD16_BYTE( "9461.bin", 0x000001, 0x020000, CRC(b7515266) SHA1(d0b4d9b1dbc968f289e04398e81b585e71e3b358) )
	j6roller_sound
ROM_END

ROM_START( j6rollerc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9464.bin", 0x000000, 0x020000, CRC(4abc4a43) SHA1(85b6379c356d02ba3ab2e7bd4bbc628d95f0b750) )
	ROM_LOAD16_BYTE( "9461.bin", 0x000001, 0x020000, CRC(b7515266) SHA1(d0b4d9b1dbc968f289e04398e81b585e71e3b358) )
	j6roller_sound
ROM_END

ROM_START( j6rollerd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "roco15p1.bin", 0x000000, 0x020000, CRC(ef1104f6) SHA1(5f61e4d22f17f761280eb8e932792d211d9928d3) )
	ROM_LOAD16_BYTE( "roco15p2.bin", 0x000001, 0x020000, CRC(9b3d316d) SHA1(6b66e458cd53c1527c2b295e898ead83720a7d99) )
	j6roller_sound
ROM_END

ROM_START( j6rollere )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rc10v8-1.bin", 0x000001, 0x020000, CRC(8d0fac13) SHA1(44d9dbb5f9ea10068b5ffea972ee0e11ad3b6275) )
	ROM_LOAD16_BYTE( "rc10v8-2.bin", 0x000000, 0x020000, CRC(3c032987) SHA1(4fff2dd84c22ad10306d7712340857b5703b8f1b) )
	j6roller_sound
ROM_END

ROM_START( j6rollerf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rc.p1.bin", 0x000000, 0x020000, CRC(075a5c93) SHA1(ea1c6dac0af35d25c58fe8ca8e442a364cdb92e7) )
	ROM_LOAD16_BYTE( "rc.p2.bin", 0x000001, 0x020000, CRC(48cbbc99) SHA1(8bbb445d0e1defcac44d5637006d974aa268b8bf) )
	j6roller_sound
ROM_END

ROM_START( j6rollerg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8137.bin", 0x000000, 0x020000, CRC(ecc408ab) SHA1(12608c8645f02ea7d2d368907fd36be90ea5da56) )
	ROM_LOAD16_BYTE( "rc.p2.bin", 0x000001, 0x020000, CRC(48cbbc99) SHA1(8bbb445d0e1defcac44d5637006d974aa268b8bf) )
	j6roller_sound
ROM_END

ROM_START( j6rollerh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8138.bin", 0x000000, 0x020000, CRC(559e9cf2) SHA1(b93c69fed10b61a1b8d9988f69ac8a980b1ec57c) )
	ROM_LOAD16_BYTE( "rc.p2.bin", 0x000001, 0x020000, CRC(48cbbc99) SHA1(8bbb445d0e1defcac44d5637006d974aa268b8bf) )
	j6roller_sound
ROM_END

ROM_START( j6rolleri )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8139.bin", 0x000000, 0x020000, CRC(65372d1b) SHA1(e4aa2350989b889e70785f368d834f422928d225) )
	ROM_LOAD16_BYTE( "rc.p2.bin", 0x000001, 0x020000, CRC(48cbbc99) SHA1(8bbb445d0e1defcac44d5637006d974aa268b8bf) )
	j6roller_sound
ROM_END

ROM_START( j6rollerj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rollercoaster10_p1.bin", 0x000000, 0x020000, CRC(566a3d2c) SHA1(410e6ef59a3af9751e59b539affde95c8d94ba31) )
	ROM_LOAD16_BYTE( "rollercoaster10_p2.bin", 0x000001, 0x020000, CRC(4ea94876) SHA1(52be24fa61431ecf3f1a206ddfb5b5a52fc9ad0a) )
	j6roller_sound
ROM_END

ROM_START( j6rollerk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7368.bin", 0x000000, 0x020000, CRC(6fe1365b) SHA1(50271888ccd4395938721f2026ecfefd342c5209) )
	ROM_LOAD16_BYTE( "7369.bin", 0x000001, 0x020000, CRC(8ab98caa) SHA1(c9bb582917b4a6be477d592d6cbd28b5f7552a26) )
	j6roller_sound
ROM_END

ROM_START( j6rollerl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7370.bin", 0x000000, 0x020000, CRC(847f6263) SHA1(b072b54ccea0a37cd4aa027bb869dd82db389360) )
	ROM_LOAD16_BYTE( "7369.bin", 0x000001, 0x020000, CRC(8ab98caa) SHA1(c9bb582917b4a6be477d592d6cbd28b5f7552a26) )
	j6roller_sound
ROM_END

ROM_START( j6rollerm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7371.bin", 0x000000, 0x020000, CRC(3d25f63a) SHA1(b37ef0c4c2cd8c948407a8767636b1a6115a1d94) )
	ROM_LOAD16_BYTE( "7369.bin", 0x000001, 0x020000, CRC(8ab98caa) SHA1(c9bb582917b4a6be477d592d6cbd28b5f7552a26) )
	j6roller_sound
ROM_END

ROM_START( j6rollern )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7372.bin", 0x000000, 0x020000, CRC(63ac986a) SHA1(2b4f7f59179ea9a239986c7a87e229a506ca006e) )
	ROM_LOAD16_BYTE( "7369.bin", 0x000001, 0x020000, CRC(8ab98caa) SHA1(c9bb582917b4a6be477d592d6cbd28b5f7552a26) )
	j6roller_sound
ROM_END

ROM_START( j6rollero )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rc82.1p1", 0x000000, 0x020000, CRC(368f1887) SHA1(7b2523ab746fc46931895ca1eb81e7d5d9a1c864) )
	ROM_LOAD16_BYTE( "rc82.1p2", 0x000001, 0x020000, CRC(dc0f8c62) SHA1(3a4b1bd698c4e14c8c794b3bfb5fa9bc631475cc) )
	j6roller_sound
ROM_END

ROM_START( j6rollerp )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rc82.1ac", 0x000000, 0x020000, CRC(3ac2b6b6) SHA1(ea71c6acf5cd6cbacdc55df6897b0dc6560e92fe) )
	ROM_LOAD16_BYTE( "rc82.1p2", 0x000001, 0x020000, CRC(dc0f8c62) SHA1(3a4b1bd698c4e14c8c794b3bfb5fa9bc631475cc) )
	j6roller_sound
ROM_END



#define j6rccls_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */
ROM_START( j6rccls )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clrc-41.bin", 0x000000, 0x020000, CRC(6f312407) SHA1(c077276d18f50340989a20fe208ac84801895dc1) )
	ROM_LOAD16_BYTE( "clrc-42.bin", 0x000001, 0x020000, CRC(9d3c34b6) SHA1(6636bb33c79d2b8a2d570b76a60b195db57336ae) )
	j6rccls_sound
ROM_END

ROM_START( j6rcclsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clrc-4a1.bin", 0x000000, 0x020000, CRC(3df5e466) SHA1(c2868ce1ac76feed5cce5f4304ce0b1e2814a763) )
	ROM_LOAD16_BYTE( "clrc-42.bin", 0x000001, 0x020000, CRC(9d3c34b6) SHA1(6636bb33c79d2b8a2d570b76a60b195db57336ae) )
	j6rccls_sound
ROM_END

ROM_START( j6rcclsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "clrc-4p1.bin", 0x000000, 0x020000, CRC(84af703f) SHA1(e336f188bebe37dfa673af49913394eb59bf8a2d) )
	ROM_LOAD16_BYTE( "clrc-42.bin", 0x000001, 0x020000, CRC(9d3c34b6) SHA1(6636bb33c79d2b8a2d570b76a60b195db57336ae) )
	j6rccls_sound
ROM_END


#define j6royfls_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "rofl-so.bin", 0x000000, 0x080000, CRC(c86bce52) SHA1(ba9b3f73e7418710737d3ace25ee2747d5685d8e) ) /* aka rflushc.as1 */

ROM_START( j6royfls )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20661.bin", 0x000000, 0x020000, CRC(f778f856) SHA1(7dd33fd41c81f6131f37d0d6fedccd36d6413eb8) )
	ROM_LOAD16_BYTE( "20662.bin", 0x000001, 0x020000, CRC(486f1e68) SHA1(ce1c3ba4d9031950db313e7179a1126a920e48e4) )
	j6royfls_sound
ROM_END

ROM_START( j6royflsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20663.bin", 0x000000, 0x020000, CRC(1ce6ac6e) SHA1(0ae838c2f5629d167920ddcafe402b39b6ebd091) )
	ROM_LOAD16_BYTE( "20662.bin", 0x000001, 0x020000, CRC(486f1e68) SHA1(ce1c3ba4d9031950db313e7179a1126a920e48e4) )
	j6royfls_sound
ROM_END

ROM_START( j6royflsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20664.bin", 0x000000, 0x020000, CRC(33a21b46) SHA1(b555688000f13910c60b38016d2ce546cc6b97bc) )
	ROM_LOAD16_BYTE( "20662.bin", 0x000001, 0x020000, CRC(486f1e68) SHA1(ce1c3ba4d9031950db313e7179a1126a920e48e4) )
	j6royfls_sound
ROM_END

ROM_START( j6royflsc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rfcl-4g1.bin", 0x000000, 0x020000, CRC(951589de) SHA1(2f4379e5cb38909c492f7ec96fdcfd0f71cd055b) )
	ROM_LOAD16_BYTE( "rfcl-4_2.bin", 0x000001, 0x020000, CRC(486f1e68) SHA1(ce1c3ba4d9031950db313e7179a1126a920e48e4) )
	j6royfls_sound
ROM_END

ROM_START( j6royflsd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rfcl-4gp.bin", 0x000000, 0x020000, CRC(7e8bdde6) SHA1(d9111880bec2e6d5b1a85c92f32c4e6599906700) )
	ROM_LOAD16_BYTE( "rfcl-4_2.bin", 0x000001, 0x020000, CRC(486f1e68) SHA1(ce1c3ba4d9031950db313e7179a1126a920e48e4) )
	j6royfls_sound
ROM_END

ROM_START( j6royflse )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "rflushc.1", 0x000000, 0x020000, CRC(c9a20e26) SHA1(34fbeb89c0b9a6d0f1cd6297245e6c56933d3981) )
	ROM_LOAD16_BYTE( "rflushc.2", 0x000001, 0x020000, CRC(02270768) SHA1(26853837b72d0529cca7dfae3488daec8d3bc998) )
	j6royfls_sound
ROM_END

#define j6samur_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6samur )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "0bff", 0x000000, 0x020000, CRC(62e39cc6) SHA1(d739d3cbf74f7e6ef2323d120095eec316bcda9b) )
	ROM_LOAD16_BYTE( "3b6c", 0x000001, 0x020000, CRC(78541fe0) SHA1(a255beab55911cb14e54abe5357e1dc8c0232755) )
	j6samur_sound
ROM_END

ROM_START( j6samura )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "0bdf", 0x000000, 0x020000, CRC(a6397fd6) SHA1(1d470d69bdbf321bd74abe7410245f3c10530625) )
	ROM_LOAD16_BYTE( "3b6c", 0x000001, 0x020000, CRC(78541fe0) SHA1(a255beab55911cb14e54abe5357e1dc8c0232755) )
	j6samur_sound
ROM_END

ROM_START( j6samurb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "0bf7", 0x000000, 0x020000, CRC(53d52402) SHA1(e038cf92b88a1f097235c358b3d6803748e54b8e) )
	ROM_LOAD16_BYTE( "3b6c", 0x000001, 0x020000, CRC(78541fe0) SHA1(a255beab55911cb14e54abe5357e1dc8c0232755) )
	j6samur_sound
ROM_END

ROM_START( j6samurc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "0bfb", 0x000000, 0x020000, CRC(7a78c0a4) SHA1(6f2ae73fa0ee6794f2e5d2e179b82de1ba41f6c8) )
	ROM_LOAD16_BYTE( "3b6c", 0x000001, 0x020000, CRC(78541fe0) SHA1(a255beab55911cb14e54abe5357e1dc8c0232755) )
	j6samur_sound
ROM_END

ROM_START( j6samurd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "0bfe", 0x000000, 0x020000, CRC(897dc8fe) SHA1(518c7cda31330725729cabb6bd76d2aea1a1a28b) )
	ROM_LOAD16_BYTE( "3b6c", 0x000001, 0x020000, CRC(78541fe0) SHA1(a255beab55911cb14e54abe5357e1dc8c0232755) )
	j6samur_sound
ROM_END


#define j6sidewd_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "sidewindersnd.bin", 0x000000, 0x080000, CRC(6e49b83b) SHA1(cba9ce8cc5dbaa0b498b2314165d4cc64c0a3881) )
ROM_START( j6sidewd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "siw-11s1.bin", 0x000000, 0x020000, CRC(d24e9e75) SHA1(22d21eb5bfe92fc61e1667345aafe7b3214c9218) )
	ROM_LOAD16_BYTE( "siw-11s2.bin", 0x000001, 0x020000, CRC(d4eee8cd) SHA1(0a3c2e19bd2202968344ca1204f7aed6250f2e34) )
	j6sidewd_sound
ROM_END

ROM_START( j6sidewda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "siw-11a1.bin", 0x000000, 0x020000, CRC(808a5e14) SHA1(7219f838928cc1dc46c85b4c17c7b7a85345dfb7) )
	ROM_LOAD16_BYTE( "siw-11s2.bin", 0x000001, 0x020000, CRC(d4eee8cd) SHA1(0a3c2e19bd2202968344ca1204f7aed6250f2e34) )
	j6sidewd_sound
ROM_END

ROM_START( j6sidewdb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "siw-11p1.bin", 0x000000, 0x020000, CRC(39d0ca4d) SHA1(3ae060b55108423662181262f1c6710787ba1975) )
	ROM_LOAD16_BYTE( "siw-11s2.bin", 0x000001, 0x020000, CRC(d4eee8cd) SHA1(0a3c2e19bd2202968344ca1204f7aed6250f2e34) )
	j6sidewd_sound
ROM_END

ROM_START( j6sidewdc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "siw-11w1.bin", 0x000000, 0x020000, CRC(b023effd) SHA1(68ed43328656c05cb8bcb2053052c6b5f8e8856c) )
	ROM_LOAD16_BYTE( "siw-11s2.bin", 0x000001, 0x020000, CRC(d4eee8cd) SHA1(0a3c2e19bd2202968344ca1204f7aed6250f2e34) )
	j6sidewd_sound
ROM_END

ROM_START( j6sidewdd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "siwi10p1", 0x000000, 0x020000, CRC(9ee164e0) SHA1(e1941810ce395f106b8f309af9e95d053d084232) )
	ROM_LOAD16_BYTE( "siwi10p2", 0x000001, 0x020000, CRC(e2751ca4) SHA1(9979fb71834f1b498628376d56cd86852276c585) )
	j6sidewd_sound
ROM_END

ROM_START( j6sidewde )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "swin15p1", 0x000000, 0x020000, CRC(aa77639c) SHA1(7051b54f5890cfb7b602e96355d7c807dbe5c0f4) )
	ROM_LOAD16_BYTE( "swin15p2", 0x000001, 0x020000, CRC(77cfa4aa) SHA1(9b4b8498b92bc503c247473befec7836bea716a9) )
	j6sidewd_sound
ROM_END


#define j6snakes_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "snla-snd.bin", 0x000000, 0x080000, CRC(7902d8ef) SHA1(222b0a18902619c9b4b29fa2485cb4e143c21bab) )
ROM_START( j6snakes )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20351.bin", 0x000000, 0x020000, CRC(7f9685fd) SHA1(b55c39ebbabd59a4c9aeea0f4337b8a629bad664) ) // aka snla.p1
	ROM_LOAD16_BYTE( "20352.bin", 0x000001, 0x020000, CRC(c51c0fb2) SHA1(4eea976fe46b35f46553cd0645ff39d7ac62d988) ) // aka snla.p2
	j6snakes_sound
ROM_END

ROM_START( j6snakesa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20353.bin", 0x000000, 0x020000, CRC(9408d1c5) SHA1(06b6fdba329f8b4854a92b61571359c8cde3fd26) )
	ROM_LOAD16_BYTE( "20352.bin", 0x000001, 0x020000, CRC(c51c0fb2) SHA1(4eea976fe46b35f46553cd0645ff39d7ac62d988) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20354.bin", 0x000000, 0x020000, CRC(2d52459c) SHA1(740d0033174bfed153e17b0efc140e380c519654) )
	ROM_LOAD16_BYTE( "20352.bin", 0x000001, 0x020000, CRC(c51c0fb2) SHA1(4eea976fe46b35f46553cd0645ff39d7ac62d988) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20355.bin", 0x000000, 0x020000, CRC(1dfbf475) SHA1(536fedcffc07c08f7214a140a4bcde4f4127af8e) )
	ROM_LOAD16_BYTE( "20352.bin", 0x000001, 0x020000, CRC(c51c0fb2) SHA1(4eea976fe46b35f46553cd0645ff39d7ac62d988) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "slad15p1", 0x000000, 0x020000, CRC(acb53439) SHA1(45e6ce2ef508655c75f836a5979740021bc93227) )
	ROM_LOAD16_BYTE( "slad15p2", 0x000001, 0x020000, CRC(3962b105) SHA1(e216abf66539e2bc19e5a67946fd3b9366a41b5d) )
	j6snakes_sound
ROM_END

ROM_START( j6snakese )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "slad17p1", 0x000000, 0x020000, CRC(06f87c27) SHA1(52f25180524fcc18c11e039937ac53350af5e90e) )
	ROM_LOAD16_BYTE( "slad17p2", 0x000001, 0x020000, CRC(2cc8a1eb) SHA1(5762ff97a7dd5e8857d506cce333f197ac7a7925) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "snla10p1", 0x000000, 0x020000, CRC(2de13a66) SHA1(73d534348408648d678609c1821d97a8877788fa) )
	ROM_LOAD16_BYTE( "snla10p2", 0x000001, 0x020000, CRC(f0116e1d) SHA1(b53351142676b50484202bde180067eec0c592e6) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "snladv-1", 0x000000, 0x020000, CRC(ced86202) SHA1(ba04b24f97a17f5d8aecd63515687e87f34029bb) )
	ROM_LOAD16_BYTE( "snladv-2", 0x000001, 0x020000, CRC(64b29222) SHA1(6e17a0ee68af644ef59abcacd5d3ad2412fdfad6) )
	j6snakes_sound
ROM_END


ROM_START( j6snakesh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "sal111.bin", 0x00000, 0x020000, CRC(e82a1072) SHA1(5175e39ad471f2fa9240364bf893a9f493b44f63) )
	ROM_LOAD16_BYTE(  "sal112.bin", 0x00001, 0x020000, CRC(a5cc9b6a) SHA1(facd8fcd8ae61b73d5b42809619bc27d8bd6ef9a) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "sal111a.bin", 0x0000, 0x020000, CRC(baeed013) SHA1(0366f27b47bae80d3f51f66baf24e2b1faad8d57) )
	ROM_LOAD16_BYTE(  "sal112.bin", 0x00001, 0x020000, CRC(a5cc9b6a) SHA1(facd8fcd8ae61b73d5b42809619bc27d8bd6ef9a) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "sal111p.bin", 0x0000, 0x020000, CRC(03b4444a) SHA1(fcd77cd44fb1e6c9094296bf21c561acb27f5ea9) )
	ROM_LOAD16_BYTE(  "sal112.bin", 0x00001, 0x020000, CRC(a5cc9b6a) SHA1(facd8fcd8ae61b73d5b42809619bc27d8bd6ef9a) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "salft1p1.bin", 0x0000, 0x020000, CRC(cf0540b1) SHA1(f225195c663e5b081af21e4e212d776a0bb08b7a) )
	ROM_LOAD16_BYTE(  "salft1p2.bin", 0x00001, 0x020000, CRC(e7ce4c21) SHA1(b6571c028710f5797607f871c7c2cb87df749131) )
	j6snakes_sound
ROM_END

ROM_START( j6snakesl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "snakes+ladders-crystal-p1.bin", 0x0001, 0x020000, CRC(4f27b6c7) SHA1(cd457f4a4e7d518d4d19f6860d6e07207afff4c6) )
	ROM_LOAD16_BYTE(  "snakes+ladders-crystal-p2.bin", 0x0000, 0x020000, CRC(3b3f5d04) SHA1(1f190296667aa599af1cc0b001be40a7b212e5d8) )
	j6snakes_sound
ROM_END




#define j6sonic_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "son_snd.bin", 0x000000, 0x080000, CRC(5cd8cf21) SHA1(82f875a59d678ef548173ee2c202e3963bc13116) )
ROM_START( j6sonic )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9440.bin", 0x000000, 0x020000, CRC(384f931e) SHA1(12bddb3dc2c1bc6c51c5ef4002673c6f45fa335c) )
	ROM_LOAD16_BYTE( "9441.bin", 0x000001, 0x020000, CRC(cba4c367) SHA1(ad926216081797b81b93eb111bf8fd50b289c9e2) )
	j6sonic_sound
ROM_END

ROM_START( j6sonica )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9442.bin", 0x000000, 0x020000, CRC(d3d1c726) SHA1(4957542e0f451fe523ed2d19e9b3df3dec126a0a) )
	ROM_LOAD16_BYTE( "9441.bin", 0x000001, 0x020000, CRC(cba4c367) SHA1(ad926216081797b81b93eb111bf8fd50b289c9e2) )
	j6sonic_sound
ROM_END

ROM_START( j6sonicb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9443.bin", 0x000000, 0x020000, CRC(6a8b537f) SHA1(1e228e3e07dfb775add02510491523501f6d374b) )
	ROM_LOAD16_BYTE( "9441.bin", 0x000001, 0x020000, CRC(cba4c367) SHA1(ad926216081797b81b93eb111bf8fd50b289c9e2) )
	j6sonic_sound
ROM_END

ROM_START( j6sonicc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9444.bin", 0x000000, 0x020000, CRC(5a22e296) SHA1(372e3bd4a0e83adc01c8882dae3c392917b84613) )
	ROM_LOAD16_BYTE( "9441.bin", 0x000001, 0x020000, CRC(cba4c367) SHA1(ad926216081797b81b93eb111bf8fd50b289c9e2) )
	j6sonic_sound
ROM_END

ROM_START( j6sonicd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "sohe8p1", 0x000000, 0x020000, CRC(3659825e) SHA1(9cd23058b244d4d0ecaaeec351a2ed4e68671688) ) // aka son15p1
	ROM_LOAD16_BYTE( "sohe8p2", 0x000001, 0x020000, CRC(e9aa2dbc) SHA1(1e8d1ad35bb55a48223a10e11e51087a12379aae) ) // aka son15p2
	j6sonic_sound
ROM_END

ROM_START( j6sonice )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "shgv7p1s.bin", 0x000000, 0x020000, CRC(55896051) SHA1(35f6856e076aead15819af7a537f8bbd6eabe3d4) )
	ROM_LOAD16_BYTE( "shgv7p2s.bin", 0x000001, 0x020000, CRC(8f128e3e) SHA1(a1c1c3ad091b4838450a151e74087453d47cf5d1) )
	j6sonic_sound
ROM_END

ROM_START( j6sonicf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "shgv7p1a.bin", 0x000000, 0x020000, CRC(074da030) SHA1(69b5c256a36940503a7405eeba68e66865319c9d) )
	ROM_LOAD16_BYTE( "shgv7p2s.bin", 0x000001, 0x020000, CRC(8f128e3e) SHA1(a1c1c3ad091b4838450a151e74087453d47cf5d1) )
	j6sonic_sound
ROM_END


ROM_START( j6sonicg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "sonic.p1", 0x000000, 0x020000, CRC(d4e65332) SHA1(60f0ef8fb5a21cfa3bfa2f8cf4f2ceebe672bb3a) )
	ROM_LOAD16_BYTE( "sonic.p2", 0x000001, 0x020000, CRC(4bf35499) SHA1(ea8ecfadc9f2c3b2000986735edb57699959f24c) )
	j6sonic_sound
ROM_END

ROM_START( j6sonich )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hdg6cp1", 0x000000, 0x010000, CRC(db73388e) SHA1(e253b4011b9e26e5634ae359924a417f437760d0) ) // something else?
	ROM_LOAD16_BYTE( "hdg6cp2", 0x000001, 0x010000, NO_DUMP )
	j6sonic_sound
ROM_END




ROM_START( j6spcinv )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "spin-31.bin", 0x000000, 0x020000, CRC(e95af475) SHA1(0439bc798739fb2bfb2931917977f505c213ea93) )
	ROM_LOAD16_BYTE( "spin-32.bin", 0x000001, 0x020000, CRC(c688fb6e) SHA1(820bd78d3ebe0134e51b322ed348be1f1874b18c) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "spaceinvaderssnd.bin", 0x000000, 0x080000, CRC(8ee10ee2) SHA1(959df6fe6b83d3671dbde4146fa2344d6a7b8b31) )
ROM_END


#define j6stards_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "star-snd.bin", 0x000000, 0x080000, CRC(d2dcd6cc) SHA1(f5a290befd41014b6aabae9fdb601d5a9766f1ba) )


ROM_START( j6stards )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9318.bin", 0x000000, 0x020000, CRC(a39d0ad8) SHA1(d41b4686fc5492e257d5913a5d66160e9a8367a9) )
	ROM_LOAD16_BYTE( "9319.bin", 0x000001, 0x020000, CRC(f5c4c5d7) SHA1(837595acad74735bb82c9fd3623813bc59c56c86) )
	j6stards_sound
ROM_END

ROM_START( j6stardsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9320.bin", 0x000000, 0x020000, CRC(48035ee0) SHA1(0d513eb953aa951a776352dd5ddfebf1c5bc61b2) )
	ROM_LOAD16_BYTE( "9319.bin", 0x000001, 0x020000, CRC(f5c4c5d7) SHA1(837595acad74735bb82c9fd3623813bc59c56c86) )
	j6stards_sound
ROM_END

ROM_START( j6stardsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9321.bin", 0x000000, 0x020000, CRC(f159cab9) SHA1(3e231a20768835b21aaecbfafe7093717bf376cf) )
	ROM_LOAD16_BYTE( "9319.bin", 0x000001, 0x020000, CRC(f5c4c5d7) SHA1(837595acad74735bb82c9fd3623813bc59c56c86) )
	j6stards_sound
ROM_END

ROM_START( j6stardsc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9322.bin", 0x000000, 0x020000, CRC(c1f07b50) SHA1(28cc1ce96bead7323b3dbdb0a97a9f4030e7ed8c) )
	ROM_LOAD16_BYTE( "9319.bin", 0x000001, 0x020000, CRC(f5c4c5d7) SHA1(837595acad74735bb82c9fd3623813bc59c56c86) )
	j6stards_sound
ROM_END


#define j6start_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6start )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "sttu-7s1.bin", 0x000000, 0x020000, CRC(7f60f498) SHA1(48070f9260cf3b5ff53613145acac6dc1511805f) )
	ROM_LOAD16_BYTE( "sttu-7_2.bin", 0x000001, 0x020000, CRC(829a2227) SHA1(d6819f029fe3778a3e9989cef1da658d7eb571ac) )
	j6start_sound
ROM_END

ROM_START( j6starta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "sttu-7p1.bin", 0x000000, 0x020000, CRC(94fea0a0) SHA1(64d8748865d650311f8b42aeac03cd38c4cdfaa1) )
	ROM_LOAD16_BYTE( "sttu-7_2.bin", 0x000001, 0x020000, CRC(829a2227) SHA1(d6819f029fe3778a3e9989cef1da658d7eb571ac) )
	j6start_sound
ROM_END


#define j6strk10_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */
ROM_START( j6strk10 )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20178.bin", 0x000000, 0x020000, CRC(35135814) SHA1(250c0835611be4eaabab4c59288d9fdeafdc6ca5) )
	ROM_LOAD16_BYTE( "20179.bin", 0x000001, 0x020000, CRC(c515abce) SHA1(0c8fb9d390f1d3f646b6d6b4177a5fa929c9067e) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10a )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20180.bin", 0x000000, 0x020000, CRC(de8d0c2c) SHA1(5c1bf58801ca9113f86aff5fa4c7f2e015fef2c0) )
	ROM_LOAD16_BYTE( "20179.bin", 0x000001, 0x020000, CRC(c515abce) SHA1(0c8fb9d390f1d3f646b6d6b4177a5fa929c9067e) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10b )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20181.bin", 0x000000, 0x020000, CRC(67d79875) SHA1(a5fbdb71487219e4432a0c94c00e341ebb124358) )
	ROM_LOAD16_BYTE( "20179.bin", 0x000001, 0x020000, CRC(c515abce) SHA1(0c8fb9d390f1d3f646b6d6b4177a5fa929c9067e) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10c )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9490.bin", 0x000000, 0x020000, CRC(847c50ae) SHA1(0d8127bc86889426662944c6b0afb1a165343ee2) )
	ROM_LOAD16_BYTE( "9491.bin", 0x000001, 0x020000, CRC(f9f8cbd8) SHA1(0975a5e1b778158e9b68a40abcfaa06647e61378) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10d )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9492.bin", 0x000000, 0x020000, CRC(6fe20496) SHA1(a538ba97595345fddb6936082fb5f97e6b1e9054) )
	ROM_LOAD16_BYTE( "9491.bin", 0x000001, 0x020000, CRC(f9f8cbd8) SHA1(0975a5e1b778158e9b68a40abcfaa06647e61378) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10e )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9493.bin", 0x000000, 0x020000, CRC(d6b890cf) SHA1(9a9680026bdfa4965c3bb438c64f02802782b879) )
	ROM_LOAD16_BYTE( "9491.bin", 0x000001, 0x020000, CRC(f9f8cbd8) SHA1(0975a5e1b778158e9b68a40abcfaa06647e61378) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10f )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1n.bin", 0x000000, 0x020000, CRC(125f3f20) SHA1(ca525936ada9a1fb218c1f5685c2adb58782d86c) )
	ROM_LOAD16_BYTE( "prom2.bin", 0x000001, 0x020000, CRC(384b5865) SHA1(26f731b6b418fdc3bb33d27237c7e91fbf9f7aff) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10g )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1p.bin", 0x000000, 0x020000, CRC(f9c16b18) SHA1(d8cc65743e16ee4f0fa7c0e01c250912ffddb51e) )
	ROM_LOAD16_BYTE( "prom2.bin", 0x000001, 0x020000, CRC(384b5865) SHA1(26f731b6b418fdc3bb33d27237c7e91fbf9f7aff) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10h )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1n_0.bin", 0x000000, 0x020000, CRC(352c5751) SHA1(8d486cc24aba1d04ea50aeaf7b6004575c4d83e0) )
	ROM_LOAD16_BYTE( "prom2_0.bin", 0x000001, 0x020000, CRC(7109053a) SHA1(5381771e52f0333abbf3789492bd8fc6be53eea4) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10i )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1p_0.bin", 0x000000, 0x020000, CRC(deb20369) SHA1(14894481736f106c54050d63b243f8fb8c2d5142) )
	ROM_LOAD16_BYTE( "prom2_0.bin", 0x000001, 0x020000, CRC(7109053a) SHA1(5381771e52f0333abbf3789492bd8fc6be53eea4) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10j )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1n_7.bin", 0x000000, 0x020000, CRC(f750b225) SHA1(d02216abd1bbc0b8010c7fcd1c29ca3949196ff6) )
	ROM_LOAD16_BYTE( "prom2_7.bin", 0x000001, 0x020000, CRC(deab0a4e) SHA1(aeba0182906332996efe79dfc56a14fdd087940e) )
	j6strk10_sound
ROM_END

ROM_START( j6strk10k )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1p_7.bin", 0x000000, 0x020000, CRC(1ccee61d) SHA1(dcd4942bc66d3109ea540fed50148c0e355b803d) )
	ROM_LOAD16_BYTE( "prom2_7.bin", 0x000001, 0x020000, CRC(deab0a4e) SHA1(aeba0182906332996efe79dfc56a14fdd087940e) )
	j6strk10_sound
ROM_END


#define j6supbrk_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "subr-snd.bin", 0x000000, 0x080000, CRC(d2439c80) SHA1(8c80927e0d0c139293bd588fad15941bfb54674d) )
ROM_START( j6supbrk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-2_1.bin", 0x000000, 0x020000, CRC(85222c75) SHA1(1fbed0f474a7ccfb523270b913a8adb41c8be388) )
	ROM_LOAD16_BYTE( "subr-2_2.bin", 0x000001, 0x020000, CRC(bd0c6f72) SHA1(df0daea8c89b1f1c7a12fd7e4e54ad6a562bb1df) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-2a1.bin", 0x000000, 0x020000, CRC(d7e6ec14) SHA1(41b89a59e311bb532a3e1d6632a56e4a87105abf) )
	ROM_LOAD16_BYTE( "subr-2_2.bin", 0x000001, 0x020000, CRC(bd0c6f72) SHA1(df0daea8c89b1f1c7a12fd7e4e54ad6a562bb1df) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrkb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-2n1.bin", 0x000000, 0x020000, CRC(b41494b1) SHA1(6aa0d8cc14a6851e887600d8f9612ddca7a58917) )
	ROM_LOAD16_BYTE( "subr-2_2.bin", 0x000001, 0x020000, CRC(bd0c6f72) SHA1(df0daea8c89b1f1c7a12fd7e4e54ad6a562bb1df) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrkc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-2np.bin", 0x000000, 0x020000, CRC(5f8ac089) SHA1(b37e4d778d72720159f2f94afb0b3797b0282d10) )
	ROM_LOAD16_BYTE( "subr-2_2.bin", 0x000001, 0x020000, CRC(bd0c6f72) SHA1(df0daea8c89b1f1c7a12fd7e4e54ad6a562bb1df) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrkd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-2p1.bin", 0x000000, 0x020000, CRC(6ebc784d) SHA1(04282ac643adbf1051477595856d45329c5b0c08) )
	ROM_LOAD16_BYTE( "subr-2_2.bin", 0x000001, 0x020000, CRC(bd0c6f72) SHA1(df0daea8c89b1f1c7a12fd7e4e54ad6a562bb1df) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrke )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-2w1.bin", 0x000000, 0x020000, CRC(e74f5dfd) SHA1(6a67d02298abd442df81dac7a797f16b8389507a) )
	ROM_LOAD16_BYTE( "subr-2_2.bin", 0x000001, 0x020000, CRC(bd0c6f72) SHA1(df0daea8c89b1f1c7a12fd7e4e54ad6a562bb1df) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrkf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-3_1.bin", 0x000000, 0x020000, CRC(3915930d) SHA1(f3ddbd23123722ff0dd8abb9ec02ecf5a49e2387) )
	ROM_LOAD16_BYTE( "subr-3_2.bin", 0x000001, 0x020000, CRC(6fb4c56d) SHA1(356caaa4b9af3f273a875ead7ee62ec76f7e9602) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrkg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-3a1.bin", 0x000000, 0x020000, CRC(6bd1536c) SHA1(37679582bd1ea213b7a744b8da0ad5dd5e4d3cb5) )
	ROM_LOAD16_BYTE( "subr-3_2.bin", 0x000001, 0x020000, CRC(6fb4c56d) SHA1(356caaa4b9af3f273a875ead7ee62ec76f7e9602) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrkh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-3n1.bin", 0x000000, 0x020000, CRC(08232bc9) SHA1(4cf6d8b1c849b19030eec815c6e9a938dc7f3e91) )
	ROM_LOAD16_BYTE( "subr-3_2.bin", 0x000001, 0x020000, CRC(6fb4c56d) SHA1(356caaa4b9af3f273a875ead7ee62ec76f7e9602) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrki )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr-3p1.bin", 0x000000, 0x020000, CRC(d28bc735) SHA1(3a7e55c05cd8618b8fe8d887d3c071054123e21e) )
	ROM_LOAD16_BYTE( "subr-3_2.bin", 0x000001, 0x020000, CRC(6fb4c56d) SHA1(356caaa4b9af3f273a875ead7ee62ec76f7e9602) )
	j6supbrk_sound
ROM_END

ROM_START( j6supbrkj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "subr3np1.bin", 0x000000, 0x020000, CRC(e3bd7ff1) SHA1(3b6105fc57ba246837829e1e01a79372603287f3) )
	ROM_LOAD16_BYTE( "subr-3_2.bin", 0x000001, 0x020000, CRC(6fb4c56d) SHA1(356caaa4b9af3f273a875ead7ee62ec76f7e9602) )
	j6supbrk_sound
ROM_END


ROM_START( j6swpdrp )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "swoptillyadrop.p1", 0x000000, 0x020000, CRC(8e372c50) SHA1(7264eb0072f2863a301e522eea83fcc5585538dc) )
	ROM_LOAD16_BYTE( "swoptillyadrop.p2", 0x000001, 0x020000, CRC(c0d16c07) SHA1(a19894fdedf01e9b57e82c473e4adbf2ad95a27e) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "swoptillyadropsnd.bin", 0x000000, 0x080000, CRC(2ed0d1de) SHA1(76a8ae25f4932d84a4bdb6e67c30d366e4850d0c) )
ROM_END


ROM_START( j6bags )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbf.p1", 0x000000, 0x020000, CRC(36605488) SHA1(0b8895288122427ba552bf3a3f5ec123c44a146c) )
	ROM_LOAD16_BYTE( "tbf.p2", 0x000001, 0x020000, CRC(b368c27c) SHA1(d83ed79205ff2f499584ab62173f753e4038752a) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "tbfsnd.bin", 0x000000, 0x080000, CRC(73c2460a) SHA1(af1aed4f56690f4f055b28c6bd5c0296b98f4f8d) )
ROM_END


#define j6roof_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6roof )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1n.bin", 0x000000, 0x020000, CRC(c00d171c) SHA1(8cd2992aa7ccb6e7d064ae736652df76030cad7b) )
	ROM_LOAD16_BYTE( "prom2.bin", 0x000001, 0x020000, CRC(6782d773) SHA1(dae453b9b4672f66228551d0c74a0d6c6690e95d) )
	j6roof_sound
ROM_END

ROM_START( j6roofa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "prom1p.bin", 0x000000, 0x020000, CRC(2b934324) SHA1(13864411cb12a68a6794fd063af725d9c130bdfe) )
	ROM_LOAD16_BYTE( "prom2.bin", 0x000001, 0x020000, CRC(6782d773) SHA1(dae453b9b4672f66228551d0c74a0d6c6690e95d) )
	j6roof_sound
ROM_END


#define j6tbirds_sound \
	ROM_REGION( 0x80000, "upd", 0 ) \
	ROM_LOAD( "tbcl-snd.bin", 0x000000, 0x80000, CRC(1cc197be) SHA1(2247aa1a0e6aab7389b3222f373890f54e907361) )
ROM_START( j6tbirds )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-2_1.bin", 0x000000, 0x020000, CRC(a1ca874a) SHA1(0a12f387a9a3e261c1017922b8d6a652c696855a) )
	ROM_LOAD16_BYTE( "thbi-2_2.bin", 0x000001, 0x020000, CRC(65320620) SHA1(3db5ccf4e69d85bb8b0270b12674c64ff674834c) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbib-2a1.bin",0x000000, 0x020000, CRC(f30e472b) SHA1(e30b4c5c04f31ebf677af5f7924efc8836e6a91f) )
	ROM_LOAD16_BYTE( "thbi-2_2.bin", 0x000001, 0x020000, CRC(65320620) SHA1(3db5ccf4e69d85bb8b0270b12674c64ff674834c) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbib-2p1.bin",0x000000, 0x020000, CRC(4a54d372) SHA1(bd7924eaeab185f097283c14b3bb197403853050) )
	ROM_LOAD16_BYTE( "thbi-2_2.bin", 0x000001, 0x020000, CRC(65320620) SHA1(3db5ccf4e69d85bb8b0270b12674c64ff674834c) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-2n1.bin", 0x000000, 0x020000, CRC(90fc3f8e) SHA1(6b3e50482e8b779ad0cad8509ba76efd055d6987) )
	ROM_LOAD16_BYTE( "thbi-2_2.bin", 0x000001, 0x020000, CRC(65320620) SHA1(3db5ccf4e69d85bb8b0270b12674c64ff674834c) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi2np1.bin", 0x000000, 0x020000, CRC(7b626bb6) SHA1(ed8eefb2207f908b0c5bc3f315d014faccdfb493) )
	ROM_LOAD16_BYTE( "thbi-2_2.bin", 0x000001, 0x020000, CRC(65320620) SHA1(3db5ccf4e69d85bb8b0270b12674c64ff674834c) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdse )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-2w1.bin", 0x000000, 0x020000, CRC(c3a7f6c2) SHA1(c72a05ed21801401d5aec610d30482560b00baee) )
	ROM_LOAD16_BYTE( "thbi-2_2.bin", 0x000001, 0x020000, CRC(65320620) SHA1(3db5ccf4e69d85bb8b0270b12674c64ff674834c) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-3_1.bin", 0x000000, 0x020000, CRC(ecbdd85c) SHA1(626306a72d658af6ecfa4b65212cf8aa35630539) )
	ROM_LOAD16_BYTE( "thbi-3_2.bin", 0x000001, 0x020000, CRC(61e98f71) SHA1(315044637bde8ab862af08bf9825917c87fcc77d) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-3n1.bin", 0x000000, 0x020000, CRC(dd8b6098) SHA1(79d2fcf128fbfe14bd62cc649825fefc10d386a5) )
	ROM_LOAD16_BYTE( "thbi-3_2.bin", 0x000001, 0x020000, CRC(61e98f71) SHA1(315044637bde8ab862af08bf9825917c87fcc77d) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi3np1.bin", 0x000000, 0x020000, CRC(361534a0) SHA1(e2ad00f1698a69b71084dba99b75f40a5f59aa20) )
	ROM_LOAD16_BYTE( "thbi-3_2.bin", 0x000001, 0x020000, CRC(61e98f71) SHA1(315044637bde8ab862af08bf9825917c87fcc77d) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsi )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbib-3a1.bin",0x000000, 0x020000, CRC(be79183d) SHA1(13936aafbb5420748d74b531cbef0a1c39f9be5d) )
	ROM_LOAD16_BYTE( "thbi-3_2.bin", 0x000001, 0x020000, CRC(61e98f71) SHA1(315044637bde8ab862af08bf9825917c87fcc77d) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbib-3p1.bin",0x000000, 0x020000, CRC(07238c64) SHA1(8653eb78537c22a89bcd90e69ddc226dd25dbd76) )
	ROM_LOAD16_BYTE( "thbi-3_2.bin", 0x000001, 0x020000, CRC(61e98f71) SHA1(315044637bde8ab862af08bf9825917c87fcc77d) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-21.bin", 0x000000, 0x020000, CRC(86c15e0f) SHA1(d4ad355162f66ca43d7a0e9081e7cf328f21a505) )
	ROM_LOAD16_BYTE( "thbi-22.bin", 0x000001, 0x020000, CRC(5588d052) SHA1(ee24e97b21d3b21571c619e823e047b646f1dfc4) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-2a1.bin", 0x000000, 0x020000, CRC(d4059e6e) SHA1(fe086c163591f7ef81c77181fae32f4b0d780ceb) )
	ROM_LOAD16_BYTE( "thbi-22.bin", 0x000001, 0x020000, CRC(5588d052) SHA1(ee24e97b21d3b21571c619e823e047b646f1dfc4) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsm )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-2p1.bin", 0x000000, 0x020000, CRC(6d5f0a37) SHA1(710b7a197047867030159c2e9f3c450ccf236952) )
	ROM_LOAD16_BYTE( "thbi-22.bin", 0x000001, 0x020000, CRC(5588d052) SHA1(ee24e97b21d3b21571c619e823e047b646f1dfc4) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-31.bin", 0x000000, 0x020000, BAD_DUMP CRC(7cdd730d) SHA1(8e73e1a745b82bab0cf767bfdc42751f0f32d0d5) ) // either bad, or doesn't pair with this ROM
	ROM_LOAD16_BYTE( "thbi-32.bin", 0x000001, 0x020000, CRC(f02d61db) SHA1(0d2a59e2b3ec68999340732a57f69eb516b6c74e) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdso )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-3p1.bin", 0x000000, 0x020000, CRC(9c814ae1) SHA1(42047fd73bc02da2add5f45882e8977a542988c4) )
	ROM_LOAD16_BYTE( "thbi-32.bin", 0x000001, 0x020000, CRC(f02d61db) SHA1(0d2a59e2b3ec68999340732a57f69eb516b6c74e) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsp )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-3a1.bin", 0x000000, 0x020000, CRC(25dbdeb8) SHA1(68f1e467cf12f711e639d19de1afdf18ebbffe3b) )
	ROM_LOAD16_BYTE( "thbi-32.bin", 0x000001, 0x020000, CRC(f02d61db) SHA1(0d2a59e2b3ec68999340732a57f69eb516b6c74e) )
	j6tbirds_sound
ROM_END

ROM_START( j6tbirdsq )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "thbi-31_bin", 0x0000, 0x020000, CRC(771f1ed9) SHA1(aea7208b3f5887ca2a25842ce18bac62cf37a955) ) /* hacked / bad? */
	ROM_LOAD16_BYTE( "thbi-32.bin", 0x000001, 0x020000, CRC(f02d61db) SHA1(0d2a59e2b3ec68999340732a57f69eb516b6c74e) )
	j6tbirds_sound
ROM_END



// different sound rom..
ROM_START( j6tbirdsr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbirdsp.p1", 0x000000, 0x020000, CRC(e59977e3) SHA1(69d7628ec50691f9363685de49013c9303e9bcc6) )
	ROM_LOAD16_BYTE( "tbirdsp.p2", 0x000001, 0x020000, CRC(7932c4ee) SHA1(c90fc982c433429aeafc4c787905d950209189f7) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "tbirdsps.bin", 0x000000, 0x080000, CRC(a0158242) SHA1(e0aac98f493f229eca2dbf934f938c6fa250fa6b) )
ROM_END

#define j6tbrdcl_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "tbcl-snd.bin", 0x000000, 0x80000, CRC(1cc197be) SHA1(2247aa1a0e6aab7389b3222f373890f54e907361) )

ROM_START( j6tbrdcl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-b_1.bin", 0x000000, 0x020000, CRC(6a034cfd) SHA1(2b79072cd90b40f369f1f00379b6249d1f3578ed) ) // aka tbirds.b1
	ROM_LOAD16_BYTE( "tbcl-b_2.bin", 0x000001, 0x020000, CRC(45d1343a) SHA1(afbb33aa21d2e9834bdfb7c21124adbe3222b48b) ) // aka tbirds.b2
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdcla )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-bf1.bin", 0x000000, 0x020000, CRC(aed9afed) SHA1(6c4cbbc9344448b1a5ac14dc680049a7999e7d68) )
	ROM_LOAD16_BYTE( "tbcl-b_2.bin", 0x000001, 0x020000, CRC(45d1343a) SHA1(afbb33aa21d2e9834bdfb7c21124adbe3222b48b) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdclb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-bn1.bin", 0x000000, 0x020000, CRC(086e3d75) SHA1(de51cb464f4e6a3174d950e62bd0dbb8d004af72) )
	ROM_LOAD16_BYTE( "tbcl-b_2.bin", 0x000001, 0x020000, CRC(45d1343a) SHA1(afbb33aa21d2e9834bdfb7c21124adbe3222b48b) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdclc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-bp1.bin", 0x000000, 0x020000, CRC(819d18c5) SHA1(af41757ddba761db7564ad9531f85fb852e6c0de) )
	ROM_LOAD16_BYTE( "tbcl-b_2.bin", 0x000001, 0x020000, CRC(45d1343a) SHA1(afbb33aa21d2e9834bdfb7c21124adbe3222b48b) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdcld )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbclbnp1.bin", 0x000000, 0x020000, CRC(e3f0694d) SHA1(cdc3853aba0e226c24e3b7ad2acd9527405b5bb0) )
	ROM_LOAD16_BYTE( "tbcl-b_2.bin", 0x000001, 0x020000, CRC(45d1343a) SHA1(afbb33aa21d2e9834bdfb7c21124adbe3222b48b) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdcle )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "f0ff", 0x000000, 0x020000, CRC(0ae70f24) SHA1(c8cf9c654b6eac5a59454f5c7db2e1bc53c37972) )
	ROM_LOAD16_BYTE( "c001", 0x000001, 0x020000, CRC(499d9ed8) SHA1(929ea1d7c2aa4749666002f54727cddb06ecc280) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdclf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "f0fe", 0x000000, 0x020000, CRC(e1795b1c) SHA1(7bb080be3a05e48852caf4ecd1f1b2500823d7c6) )
	ROM_LOAD16_BYTE( "c001", 0x000001, 0x020000, CRC(499d9ed8) SHA1(929ea1d7c2aa4749666002f54727cddb06ecc280) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdclg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fofb", 0x000000, 0x020000, CRC(127c5346) SHA1(77d3cafc157d6f47cfde6aa023932107af18c0fe) )
	ROM_LOAD16_BYTE( "c001", 0x000001, 0x020000, CRC(499d9ed8) SHA1(929ea1d7c2aa4749666002f54727cddb06ecc280) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdclh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "f0df", 0x000000, 0x020000, CRC(ce3dec34) SHA1(5675ceeb8a3d8f856af59cbb5ea348acf68bb33a) )
	ROM_LOAD16_BYTE( "c001", 0x000001, 0x020000, CRC(499d9ed8) SHA1(929ea1d7c2aa4749666002f54727cddb06ecc280) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdcli )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-e1.bin", 0x000000, 0x020000, CRC(04ef59bd) SHA1(f6d07217535238c2e6c7c8a3e78d5680944fe850) )
	ROM_LOAD16_BYTE( "tbcl-e2.bin", 0x000001, 0x020000, CRC(dd05618b) SHA1(14f6aa577d5a33ec3b744aea94c46dcecc9ae810) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdclj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-ee1.bin", 0x000000, 0x020000, CRC(1c7405df) SHA1(2d1e5bb1af37699765d0b5d8ecc8e39058835523) )
	ROM_LOAD16_BYTE( "tbcl-e2.bin", 0x000001, 0x020000, CRC(dd05618b) SHA1(14f6aa577d5a33ec3b744aea94c46dcecc9ae810) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdclk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-ef1.bin", 0x000000, 0x020000, CRC(c035baad) SHA1(65ebaa609dd0b6251a7697f48571bd9bf934c8ca) )
	ROM_LOAD16_BYTE( "tbcl-e2.bin", 0x000001, 0x020000, CRC(dd05618b) SHA1(14f6aa577d5a33ec3b744aea94c46dcecc9ae810) )
	j6tbrdcl_sound
ROM_END

ROM_START( j6tbrdcll )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tbcl-ep1.bin", 0x000000, 0x020000, CRC(ef710d85) SHA1(befbd30fc8807ca80c5fa9963a8025860624ac51) )
	ROM_LOAD16_BYTE( "tbcl-e2.bin", 0x000001, 0x020000, CRC(dd05618b) SHA1(14f6aa577d5a33ec3b744aea94c46dcecc9ae810) )
	j6tbrdcl_sound
ROM_END


#define j6tomb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "tr_snd.bin", 0x000000, 0x080000, CRC(6cd91050) SHA1(6818e59fc52b9776ee40bf7f2a8fca2f74343335) )
ROM_START( j6tomb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tora-41.bin", 0x000000, 0x020000, CRC(80956d40) SHA1(0864e5fa7bb41dca2128566ccd80c1208dea0157) )
	ROM_LOAD16_BYTE( "tora-42.bin", 0x000001, 0x020000, CRC(4f4feeb4) SHA1(7d267a395e5450f71d0788a7b493bab133a622a0) )
	j6tomb_sound
ROM_END

ROM_START( j6tomba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tora-4a1.bin", 0x000000, 0x020000, CRC(d251ad21) SHA1(d9bdd1d569e2a76780d59b72d6416f5983f1102b) )
	ROM_LOAD16_BYTE( "tora-42.bin", 0x000001, 0x020000, CRC(4f4feeb4) SHA1(7d267a395e5450f71d0788a7b493bab133a622a0) )
	j6tomb_sound
ROM_END

ROM_START( j6tombb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tora-4p1.bin", 0x000000, 0x020000, CRC(6b0b3978) SHA1(a4543c0ea19af39acc7c9ffd498079fbc5374d54) )
	ROM_LOAD16_BYTE( "tora-42.bin", 0x000001, 0x020000, CRC(4f4feeb4) SHA1(7d267a395e5450f71d0788a7b493bab133a622a0) )
	j6tomb_sound
ROM_END

ROM_START( j6tombc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tr_1.bin", 0x000000, 0x020000, CRC(d93f00b4) SHA1(edd3136fd52fa97de4dc45ebf72d7eaa85a8687f) )
	ROM_LOAD16_BYTE( "tr_2.bin", 0x000001, 0x020000, CRC(78cdf764) SHA1(61643d684b63916eaef2cd54535bae2ed575545f) )
	j6tomb_sound
ROM_END

ROM_START( j6tombd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tora-3a1.bin", 0x000000, 0x020000, CRC(8bfbc0d5) SHA1(a473fa489f1a238790c240085aa7971b7e965c75) )
	ROM_LOAD16_BYTE( "tr_2.bin", 0x000001, 0x020000, CRC(78cdf764) SHA1(61643d684b63916eaef2cd54535bae2ed575545f) )
	j6tomb_sound
ROM_END

ROM_START( j6tombe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tora-3n1.bin", 0x000000, 0x020000, CRC(e809b870) SHA1(f291b1b6b3926c584ea055f4459592d6cc24f5be) )
	ROM_LOAD16_BYTE( "tr_2.bin", 0x000001, 0x020000, CRC(78cdf764) SHA1(61643d684b63916eaef2cd54535bae2ed575545f) )
	j6tomb_sound
ROM_END

ROM_START( j6tombf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tora-3np.bin", 0x000000, 0x020000, CRC(0397ec48) SHA1(ff26c4696e4852bd5c1c67ed89ef2c54f84c2c69) )
	ROM_LOAD16_BYTE( "tr_2.bin", 0x000001, 0x020000, CRC(78cdf764) SHA1(61643d684b63916eaef2cd54535bae2ed575545f) )
	j6tomb_sound
ROM_END

ROM_START( j6tombg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tora-3p1.bin", 0x000000, 0x020000, CRC(32a1548c) SHA1(a365cae2a1e3ebfc9b2ca401ba44cbbc72d4b822) )
	ROM_LOAD16_BYTE( "tr_2.bin", 0x000001, 0x020000, CRC(78cdf764) SHA1(61643d684b63916eaef2cd54535bae2ed575545f) )
	j6tomb_sound
ROM_END



ROM_START( j6topflg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "topflight.p1", 0x000000, 0x020000, CRC(f7e45700) SHA1(35034fa03ffb1c1c46becb6b4f194a5226d2db8d) )
	ROM_LOAD16_BYTE( "topflight.p2", 0x000001, 0x020000, CRC(98d1c5d5) SHA1(37497b0abd1586fda092bfabedd022a304dc00b0) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "topflightsnd.bin", 0x000000, 0x080000, CRC(c8e5b96e) SHA1(af62461a8279c2c09e5b5f93b5dce7ef0e973de6) )
ROM_END



ROM_START( j6tutti )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "tutti_np_v240.p1", 0x000000, 0x020000, CRC(693d469e) SHA1(995e2278cb2570aae05166b640184ddcf4e12114) )
	ROM_LOAD16_BYTE( "tutti_np_v240.p2", 0x000001, 0x020000, CRC(23c46b1d) SHA1(ac0ca5ec2f2d3aa75de27d629bd3a19c02165d3d) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "61000040.bin", 0x000000, 0x080000, CRC(64963d14) SHA1(6ed36e371477ad924031a783a5adbe83acdfd7f9) )
ROM_END


#define j6twst_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "twistersnd.bin", 0x000000, 0x080000, CRC(421a7a81) SHA1(d47caeb209eb6cfc47f82162b03563b25bbdf017) )

ROM_START( j6twst )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "twister.p1", 0x000000, 0x020000, CRC(d86b391b) SHA1(cc6154c48d408295dbfbdb96e1cc364a6d93ec5f) )
	ROM_LOAD16_BYTE( "twister.p2", 0x000001, 0x020000, CRC(52e24bd7) SHA1(72c7b1e482e61b554c36f57f9aca3cf6208e4b5f) )
	j6twst_sound
ROM_END

ROM_START( j6twsta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bb7e", 0x000000, 0x020000, CRC(b61680c6) SHA1(4dec5a6ac843dc2b085f27aacdc961d3b2c80044) )
	ROM_LOAD16_BYTE( "ef39", 0x000001, 0x020000, CRC(39fbc848) SHA1(080925d9d1cf061dcbd11dd5a104ad4decaca20d) )
	j6twst_sound
ROM_END

ROM_START( j6twstb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bb9d", 0x000000, 0x020000, CRC(cb96f78f) SHA1(8847620e5feebaa413ffc3623bd7d6cac9ccb1b7) )
	ROM_LOAD16_BYTE( "ef39", 0x000001, 0x020000, CRC(39fbc848) SHA1(080925d9d1cf061dcbd11dd5a104ad4decaca20d) )
	j6twst_sound
ROM_END

ROM_START( j6twstc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bbb5", 0x000000, 0x020000, CRC(3e7aac5b) SHA1(0452a3464672033effa85e7265a0f440205a26da) )
	ROM_LOAD16_BYTE( "ef39", 0x000001, 0x020000, CRC(39fbc848) SHA1(080925d9d1cf061dcbd11dd5a104ad4decaca20d) )
	j6twst_sound
ROM_END

ROM_START( j6twstd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bbbd", 0x000000, 0x020000, CRC(0f4c149f) SHA1(56b0bb49fc2b4115b19bf989c160129daf7806f0) )
	ROM_LOAD16_BYTE( "ef39", 0x000001, 0x020000, CRC(39fbc848) SHA1(080925d9d1cf061dcbd11dd5a104ad4decaca20d) )
	j6twst_sound
ROM_END

ROM_START( j6twste )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "bbbe", 0x000000, 0x020000, CRC(e4d240a7) SHA1(a9e4f343b9f6a273de3a2748870ff7e6041e48a0) )
	ROM_LOAD16_BYTE( "ef39", 0x000001, 0x020000, CRC(39fbc848) SHA1(080925d9d1cf061dcbd11dd5a104ad4decaca20d) )
	j6twst_sound
ROM_END

ROM_START( j6twstf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "twst-b1.bin", 0x000000, 0x020000, CRC(63d2a2a0) SHA1(91352ffa2ab0cbe46ad2ab192b6c155bec24a40b) )
	ROM_LOAD16_BYTE( "twst-b2.bin", 0x000001, 0x020000, CRC(a8665dc2) SHA1(f113d584a4449924fdf3753a7e0dafe6fb27f3b8) )
	j6twst_sound
ROM_END

ROM_START( j6twstg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "twst-b1a.bin", 0x000000, 0x020000, CRC(311662c1) SHA1(87029504817d24ac54bb786231f0d17e3b54c5ed) )
	ROM_LOAD16_BYTE( "twst-b2.bin", 0x000001, 0x020000, CRC(a8665dc2) SHA1(f113d584a4449924fdf3753a7e0dafe6fb27f3b8) )
	j6twst_sound
ROM_END

ROM_START( j6twsth )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "twst-b1p.bin", 0x000000, 0x020000, CRC(884cf698) SHA1(a74f19d5910f35fc0c1196afb6f568517f1adb8b) )
	ROM_LOAD16_BYTE( "twst-b2.bin", 0x000001, 0x020000, CRC(a8665dc2) SHA1(f113d584a4449924fdf3753a7e0dafe6fb27f3b8) )
	j6twst_sound
ROM_END

ROM_START( j6twsti )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "twstb1kp.bin", 0x000000, 0x020000, CRC(b97a4e5c) SHA1(d6ebd9fa928c7f642a1bf1eb1a1b74d7862dc82c) )
	ROM_LOAD16_BYTE( "twst-b2.bin", 0x000001, 0x020000, CRC(a8665dc2) SHA1(f113d584a4449924fdf3753a7e0dafe6fb27f3b8) )
	j6twst_sound
ROM_END

ROM_START( j6twstj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "twstb1lp.bin", 0x000000, 0x020000, CRC(4c961588) SHA1(bf678dcf91378dcbd35df00134cd745a922ee0ee) )
	ROM_LOAD16_BYTE( "twst-b2.bin", 0x000001, 0x020000, CRC(a8665dc2) SHA1(f113d584a4449924fdf3753a7e0dafe6fb27f3b8) )
	j6twst_sound
ROM_END

/* different sound rom, Dutch game */
ROM_START( j6twstdt )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF ) // overdumps
	ROM_LOAD16_BYTE( "twister1.bin", 0x000000, 0x080000, CRC(7adb2a40) SHA1(fece9a0e8c20f988afb61f9bf34bb53d34ff379a) )
	ROM_LOAD16_BYTE( "twister2.bin", 0x000001, 0x080000, CRC(df09763b) SHA1(4691d050dc2adfc41fa8fef36dbc5299e0c921bf) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "twistersound.bin", 0x000000, 0x080000, CRC(66510699) SHA1(464fcd0a4279a911cf80a1e64453a477fb0b6fd4) )
ROM_END



#define j6untch_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "untouchablesnd.bin", 0x000000, 0x080000, CRC(b7fd3b56) SHA1(cd1a7f766d5ffa7c203248f5caa8bf7bd64cd4d6) )

ROM_START( j6untch )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "7121.bin", 0x000000, 0x020000, CRC(ad5c67f1) SHA1(aabb433c8490c7672cf70157ab45f12292563291) )
	ROM_LOAD16_BYTE( "7122.bin", 0x000001, 0x020000, CRC(17a560d1) SHA1(295d7de024acde88780afa98a689337855da24cd) )
	j6untch_sound
ROM_END

ROM_START( j6untcha )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "unto3bi.1", 0x0000, 0x020000, CRC(a111c9c0) SHA1(8a48fc16614231e8a9e1ea1f237f0baa13658dd9) )
	ROM_LOAD16_BYTE( "7122.bin", 0x000001, 0x020000, CRC(17a560d1) SHA1(295d7de024acde88780afa98a689337855da24cd) )
	j6untch_sound
ROM_END




#define j6pompay_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "uppo-so1.bin", 0x000000, 0x080000, CRC(60e370f8) SHA1(6b528f64ee5d00491655169bc108a7a6d383eaa5) )
ROM_START( j6pompay )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20270.bin", 0x000000, 0x020000, CRC(c214a067) SHA1(9e4d26727b32051b188c361b8ad8922cbd7a10b2) )
	ROM_LOAD16_BYTE( "20271.bin", 0x000001, 0x020000, CRC(0a45e0e2) SHA1(a2fd22b732801db08739bae88a6df549546f62d8) )
	j6pompay_sound
ROM_END

ROM_START( j6pompaya )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20272.bin", 0x000000, 0x020000, CRC(298af45f) SHA1(6b77c3357e3108d9d328c64e7e091aa81946c7de) )
	ROM_LOAD16_BYTE( "20271.bin", 0x000001, 0x020000, CRC(0a45e0e2) SHA1(a2fd22b732801db08739bae88a6df549546f62d8) )
	j6pompay_sound
ROM_END

ROM_START( j6pompayb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20273.bin", 0x000000, 0x020000, CRC(90d06006) SHA1(b48876e518b15499ac8d1fa19a4a0c56f624a3ce) )
	ROM_LOAD16_BYTE( "20271.bin", 0x000001, 0x020000, CRC(0a45e0e2) SHA1(a2fd22b732801db08739bae88a6df549546f62d8) )
	j6pompay_sound
ROM_END


ROM_START( j6pompayc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20274.bin", 0x000000, 0x020000, CRC(a079d1ef) SHA1(9097b639e457b9cc1e8333df40e6512abff3c6d4) ) // aka up_1.bin
	ROM_LOAD16_BYTE( "20271.bin", 0x000001, 0x020000, CRC(0a45e0e2) SHA1(a2fd22b732801db08739bae88a6df549546f62d8) ) // aka up_2.bin
	j6pompay_sound
ROM_END




ROM_START( j6vindal )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vindaloot.p1", 0x000000, 0x020000, CRC(141e55d6) SHA1(5472b19321fa7fbfa92e74f1849b772b593d69af) )
	ROM_LOAD16_BYTE( "vindaloot.p2", 0x000001, 0x020000, CRC(de043fa1) SHA1(1bde5d6a0da6f70ab8dcf70e549d40a0e8994b05) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "vindalootsnd.bin", 0x000000, 0x080000, CRC(2151baa1) SHA1(999915cb549e1bdae3079dea2895124becb96e2a) )
ROM_END

#define j6vivark_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "vrve-snd.bin", 0x000000, 0x080000, CRC(40374f0b) SHA1(607eac4d3caee022e61531655ded137644602939) )
ROM_START( j6vivark )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-2_1.bin", 0x000000, 0x020000, CRC(fea54f1c) SHA1(d5fcaa6417f0961bf370c90a468b5d59a17b62ba) )
	ROM_LOAD16_BYTE( "vrve-2_2.bin", 0x000001, 0x020000, CRC(eaca9d39) SHA1(3e41362fb3780a09a7647972d6df624fd8a73dba) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarka )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-2n1.bin", 0x000000, 0x020000, CRC(cf93f7d8) SHA1(67cf78c7aed9f3d8929f05326134e353d34a066b) )
	ROM_LOAD16_BYTE( "vrve-2_2.bin", 0x000001, 0x020000, CRC(eaca9d39) SHA1(3e41362fb3780a09a7647972d6df624fd8a73dba) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-2np.bin", 0x000000, 0x020000, CRC(240da3e0) SHA1(e4b7a68059602d8fdfd773e932815266db5cf2ac) )
	ROM_LOAD16_BYTE( "vrve-2_2.bin", 0x000001, 0x020000, CRC(eaca9d39) SHA1(3e41362fb3780a09a7647972d6df624fd8a73dba) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-2p1.bin", 0x000000, 0x020000, CRC(153b1b24) SHA1(43d52f3cc68874a557844a6a045eb65beecf94aa) )
	ROM_LOAD16_BYTE( "vrve-2_2.bin", 0x000001, 0x020000, CRC(eaca9d39) SHA1(3e41362fb3780a09a7647972d6df624fd8a73dba) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-2w1.bin", 0x000000, 0x020000, CRC(9cc83e94) SHA1(b14b5dc34320edcdacfe2f6a4955c0989b4013ec) )
	ROM_LOAD16_BYTE( "vrve-2_2.bin", 0x000001, 0x020000, CRC(eaca9d39) SHA1(3e41362fb3780a09a7647972d6df624fd8a73dba) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarke )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-4a1.bin", 0x000000, 0x020000, CRC(3459e107) SHA1(93559fdd19371d1d72f40b476a90a849e71e1320) )
	ROM_LOAD16_BYTE( "vrve-4_2.bin", 0x000001, 0x020000, CRC(bacf1474) SHA1(541a3afeba486d44eaf8063f729ab186bf75ec3d) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-6_1.bin", 0x000000, 0x020000, CRC(d96dafad) SHA1(68f3229fe0e2be6a258a9c0741c2f8a6f89313f5) )
	ROM_LOAD16_BYTE( "vrve-6_2.bin", 0x000001, 0x020000, CRC(524d7cd4) SHA1(3c24edd2725cd65487e74ca881c0f33c858e8095) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-6a1.bin", 0x000000, 0x020000, CRC(8ba96fcc) SHA1(6aad6ff060fc5db98a5c52860424c79fd26dbc72) )
	ROM_LOAD16_BYTE( "vrve-6_2.bin", 0x000001, 0x020000, CRC(524d7cd4) SHA1(3c24edd2725cd65487e74ca881c0f33c858e8095) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkh )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-6n1.bin", 0x000000, 0x020000, CRC(e85b1769) SHA1(abbfe158731e5c0531bf63e134be020335fa0c48) )
	ROM_LOAD16_BYTE( "vrve-6_2.bin", 0x000001, 0x020000, CRC(524d7cd4) SHA1(3c24edd2725cd65487e74ca881c0f33c858e8095) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarki )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve-6p1.bin", 0x000000, 0x020000, CRC(32f3fb95) SHA1(60ea60794db49af8b4ae1647042a3722c500caac) )
	ROM_LOAD16_BYTE( "vrve-6_2.bin", 0x000001, 0x020000, CRC(524d7cd4) SHA1(3c24edd2725cd65487e74ca881c0f33c858e8095) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkj )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrve6np1.bin", 0x000000, 0x020000, CRC(03c54351) SHA1(a4c59f2c37104074ca4ede43bcdb275aacb8d9c5) )
	ROM_LOAD16_BYTE( "vrve-6_2.bin", 0x000001, 0x020000, CRC(524d7cd4) SHA1(3c24edd2725cd65487e74ca881c0f33c858e8095) )
	j6vivark_sound
ROM_END

ROM_START( j6vivarkk )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrcl-e1.bin", 0x000000, 0x020000, CRC(f817a3c3) SHA1(e419939f22c28cbbe4c8e59398e0e7610abdbb17) ) // club set?
	ROM_LOAD16_BYTE( "vrcl-e2.bin", 0x000001, 0x020000, CRC(7b283bb4) SHA1(4841065fa9c4720184fa11227411e60eb40410bc) ) // club set?
	j6vivark_sound
ROM_END

#define j6vivrkc_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "vrcl-snds.bin", 0x000000, 0x080000, CRC(c4267ccf) SHA1(f299b63f762b420eaa5ddb024f357d7abb9fc21e) )
ROM_START( j6vivrkc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrcl-d_1s.bin", 0x000000, 0x020000, CRC(ba6a1f04) SHA1(4fbb326386d4d5dfc16e64b824b042932e0497ee) )
	ROM_LOAD16_BYTE( "vrcl-d_2s.bin", 0x000001, 0x020000, CRC(73cd7f94) SHA1(19854f8223e1614237686d42c7cbbd7853a83c62) )
	j6vivrkc_sound
ROM_END

ROM_START( j6vivrkca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrcl-df1s.bin", 0x000000, 0x020000, CRC(7eb0fc14) SHA1(8801d6ae1ab321fe9757c9bd3021db52fea8c3f1) )
	ROM_LOAD16_BYTE( "vrcl-d_2s.bin", 0x000001, 0x020000, CRC(73cd7f94) SHA1(19854f8223e1614237686d42c7cbbd7853a83c62) )
	j6vivrkc_sound
ROM_END

ROM_START( j6vivrkcb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "vrcl-dp1s.bin", 0x000000, 0x020000, CRC(51f44b3c) SHA1(389b356dfd922f0e6fe258310166dc11fabe4432) )
	ROM_LOAD16_BYTE( "vrcl-d_2s.bin", 0x000001, 0x020000, CRC(73cd7f94) SHA1(19854f8223e1614237686d42c7cbbd7853a83c62) )
	j6vivrkc_sound
ROM_END


#define j6wldkng_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "wiki-snds.bin", 0x000000, 0x080000, CRC(2ba0529c) SHA1(c4b4b80fcbb867650649a42a4abe7675eea8f848) )

ROM_START( j6wldkng )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8472.bin", 0x000000, 0x020000, CRC(69d0a138) SHA1(b06e09dc55927b2032cfa62b9c65e4f16dfc0e7a) ) // aka 8472s.bin
	ROM_LOAD16_BYTE( "8473.bin", 0x000001, 0x020000, CRC(5a39c96c) SHA1(b48a56b4cc252b51a1c02e8d04fcd79ede6e597c) ) // aka 8473s.bin
	j6wldkng_sound
ROM_END


ROM_START( j6wldknga )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8474s.bin", 0x000000, 0x020000, CRC(824ef500) SHA1(0629bd346eaf4b7c76d31b8d82fef9ee37ca98fe) )
	ROM_LOAD16_BYTE( "8473.bin", 0x000001, 0x020000, CRC(5a39c96c) SHA1(b48a56b4cc252b51a1c02e8d04fcd79ede6e597c) ) // aka 8473s.bin
	j6wldkng_sound
ROM_END


#define j6wthing_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "wildsnd.bin", 0x000000, 0x080000, CRC(a2c08185) SHA1(9ee589df284f1b803ca015fff599d229358530d4) )

ROM_START( j6wthing )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wt1_1.p1", 0x000000, 0x020000, CRC(9c01e3c7) SHA1(b8a195c56fa3c3d14940525c177cbad60582196d) )
	ROM_LOAD16_BYTE( "wt1_1.p2", 0x000001, 0x020000, CRC(1dfd6f1b) SHA1(94146203997e9107dd08db164699e3669a31454b) )
	j6wthing_sound
ROM_END

ROM_START( j6wthinga )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wt1_0.p1", 0x000000, 0x020000, CRC(5f8db0ee) SHA1(0dfc26b846c347ec513f789ecef479f2de289193) )
	ROM_LOAD16_BYTE( "wt1_0.p2", 0x000001, 0x020000, CRC(8757a772) SHA1(b5a5fc7b16f2b696935db00f962b7d663119ea38) )
	j6wthing_sound
ROM_END

ROM_START( j6wthingb ) // overdump, check if it matches above sets
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "w_thing.p1", 0x00000, 0x080000, CRC(e44d4b1b) SHA1(ff897fd60e6005cdbcee95c8621d1a1fc2be3442) )
	ROM_LOAD16_BYTE( "w_thing.p2", 0x00001, 0x080000, CRC(fd6c7571) SHA1(46dc3d8d76d70876b278050f2af4ae716205f3e7) )
	j6wthing_sound
ROM_END


#define j6wildw_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "wwe10snd.bin", 0x000000, 0x080000, CRC(00c8c428) SHA1(2e10b10093acd4c2f7051aff28a8ae976bb1425b) )
ROM_START( j6wildw )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9379.bin", 0x000000, 0x020000, CRC(533dcf84) SHA1(b1b973fc4dc0601da7587d9079e24319ecca5f1b) )
	ROM_LOAD16_BYTE( "9378.bin", 0x000001, 0x020000, CRC(2deb0db1) SHA1(a215b2d3d6dd9d2aa6c5be2ad5e0fdedafe3f557) )
	j6wildw_sound
ROM_END

ROM_START( j6wildwa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9380.bin", 0x000000, 0x020000, CRC(ea675bdd) SHA1(f328fd95c242bae08fb162b3b39f9498915639fe) )
	ROM_LOAD16_BYTE( "9378.bin", 0x000001, 0x020000, CRC(2deb0db1) SHA1(a215b2d3d6dd9d2aa6c5be2ad5e0fdedafe3f557) )
	j6wildw_sound
ROM_END

ROM_START( j6wildwb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9381.bin", 0x000000, 0x020000, CRC(daceea34) SHA1(f6af9e91d482ea47a73e1392b017a6262652271e) )
	ROM_LOAD16_BYTE( "9378.bin", 0x000001, 0x020000, CRC(2deb0db1) SHA1(a215b2d3d6dd9d2aa6c5be2ad5e0fdedafe3f557) )
	j6wildw_sound
ROM_END

ROM_START( j6wildwc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wildwest15-p1.bin", 0x000000, 0x020000, CRC(b8a39bbc) SHA1(bea3823a27e1d9b16bd9fd1a36603dfaedc8e428) ) // aka wiwe4p1
	ROM_LOAD16_BYTE( "9378.bin", 0x000001, 0x020000, CRC(2deb0db1) SHA1(a215b2d3d6dd9d2aa6c5be2ad5e0fdedafe3f557) ) // aka wildwest15-p2.bin
	j6wildw_sound
ROM_END

ROM_START( j6wildwd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wwe30nsk.1", 0x000000, 0x020000, CRC(2dc649f5) SHA1(0b792229a81a31c162d09cc35e3b05bb49e7e5c0) )
	ROM_LOAD16_BYTE( "wwe30.2", 0x000001, 0x020000, CRC(c1e36826) SHA1(15e80e9bc175610ec5840a94c17dd3472422e30b) )
	j6wildw_sound
ROM_END

ROM_START( j6wildwe )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wwe30dsk.1", 0x000000, 0x020000, CRC(c6581dcd) SHA1(3f7c8c45574da4dab3b927e1e6fc5d1c8a942ddf) )
	ROM_LOAD16_BYTE( "wwe30.2", 0x000001, 0x020000, CRC(c1e36826) SHA1(15e80e9bc175610ec5840a94c17dd3472422e30b) )
	j6wildw_sound
ROM_END

ROM_START( j6wildwf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wwe30exh.1", 0x000000, 0x020000, CRC(884fc937) SHA1(d51966519f609b2d2daf52594ef46ffc02b600ba) )
	ROM_LOAD16_BYTE( "wwe30.2", 0x000001, 0x020000, CRC(c1e36826) SHA1(15e80e9bc175610ec5840a94c17dd3472422e30b) )
	j6wildw_sound
ROM_END

ROM_START( j6wildwg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wwe30wht.1", 0x000000, 0x017000, BAD_DUMP CRC(3fa65aca) SHA1(87055c50517e20d8f9ec90539c9bd701af244951) ) /* too short, but won't matter, data ends before cut-off */
	ROM_LOAD16_BYTE( "wwe30.2", 0x000001, 0x020000, CRC(c1e36826) SHA1(15e80e9bc175610ec5840a94c17dd3472422e30b) )
	j6wildw_sound
ROM_END


#define j6wizard_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "woosnd.bin", 0x000000, 0x080000, CRC(f72bd4f4) SHA1(ef8651fe7bb5f5340e41d35ca0669cba7d9c1372) )

ROM_START( j6wizard )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20418.bin", 0x000000, 0x020000, CRC(3a90026b) SHA1(2e766b97b1e1ad8cff6f5146a45e0f063cdbb0f7) )
	ROM_LOAD16_BYTE( "20419.bin", 0x000001, 0x020000, CRC(fb11230b) SHA1(13319192825d054034b3f3af8fc0d1020925b88f) )
	j6wizard_sound
ROM_END

ROM_START( j6wizarda )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20420.bin", 0x000000, 0x020000, CRC(d10e5653) SHA1(9da581cd6e23c0cea95ea4858725e0b5b6a987c8) )
	ROM_LOAD16_BYTE( "20419.bin", 0x000001, 0x020000, CRC(fb11230b) SHA1(13319192825d054034b3f3af8fc0d1020925b88f) )
	j6wizard_sound
ROM_END

ROM_START( j6wizardb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20421.bin", 0x000000, 0x020000, CRC(6854c20a) SHA1(36dc0d70691b659b0b1a9d1cad522decd3a1155c) )
	ROM_LOAD16_BYTE( "20419.bin", 0x000001, 0x020000, CRC(fb11230b) SHA1(13319192825d054034b3f3af8fc0d1020925b88f) )
	j6wizard_sound
ROM_END

ROM_START( j6wizardc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "20422.bin", 0x000000, 0x020000, CRC(58fd73e3) SHA1(679760acf795c9c02ff1b9bb72a7947afc2aee52) )
	ROM_LOAD16_BYTE( "20419.bin", 0x000001, 0x020000, CRC(fb11230b) SHA1(13319192825d054034b3f3af8fc0d1020925b88f) )
	j6wizard_sound
ROM_END

ROM_START( j6wizardd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wizo15p1", 0x000000, 0x020000, CRC(75117690) SHA1(7ae6ef2360dd892da6eed345a2c5b5351a39b7a0) )
	ROM_LOAD16_BYTE( "wizo15p2", 0x000001, 0x020000, CRC(8954b864) SHA1(63445cfd59e64d1dcdee52185c41c32f037bf176) )
	j6wizard_sound
ROM_END

ROM_START( j6wizarde )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "wizod5p1", 0x000000, 0x020000, CRC(9c43b23d) SHA1(fac2327d5d41adc3af905642f33f541096467a2b) )
	ROM_LOAD16_BYTE( "wizod5p2", 0x000001, 0x020000, CRC(62460b0a) SHA1(89ba2bb4a9b7ea15462a86d5ccee5622850800af) )
	j6wizard_sound
ROM_END


#define j6knight_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "yourluckynightsnd.bin", 0x000000, 0x080000, CRC(53c20160) SHA1(6295797d384fd00b4d982c924a4cfbaa079e93a1) )
ROM_START( j6knight )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "yourluckynight.p1", 0x000000, 0x020000, CRC(fbf7d7d9) SHA1(885a8a5acd06bafe7df01d9e36c5315f9a7f518c) )
	ROM_LOAD16_BYTE( "yourluckynight.p2", 0x000001, 0x020000, CRC(996c1d3e) SHA1(f5127baee641619d7fb6ff66996eaa5aa4f45a88) )
	j6knight_sound
ROM_END

ROM_START( j6knighta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "6898", 0x000000, 0x020000, CRC(e6445233) SHA1(0f27531d823a5d67b8e963956313abd61163edcf) )
	ROM_LOAD16_BYTE( "278e", 0x000001, 0x020000, CRC(d3fe21b8) SHA1(a0813c4007b3058d0a23e3f658466e30bd91cc51) )
	j6knight_sound
ROM_END

ROM_START( j6knightb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "68b7", 0x000000, 0x020000, CRC(9bc4257a) SHA1(aa4ed152aa1431272c48950526eb24f01dd9db12) )
	ROM_LOAD16_BYTE( "278e", 0x000001, 0x020000, CRC(d3fe21b8) SHA1(a0813c4007b3058d0a23e3f658466e30bd91cc51) )
	j6knight_sound
ROM_END

ROM_START( j6knightc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "68d0", 0x000000, 0x020000, CRC(85b62a96) SHA1(994d2cc800efc426ccfe08a9b492278010cd64c5) )
	ROM_LOAD16_BYTE( "278e", 0x000001, 0x020000, CRC(d3fe21b8) SHA1(a0813c4007b3058d0a23e3f658466e30bd91cc51) )
	j6knight_sound
ROM_END

ROM_START( j6knightd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "68d7", 0x000000, 0x020000, CRC(5f1ec66a) SHA1(b11b591c55e440befb07ab562ff3436bd2523499) )
	ROM_LOAD16_BYTE( "278e", 0x000001, 0x020000, CRC(d3fe21b8) SHA1(a0813c4007b3058d0a23e3f658466e30bd91cc51) )
	j6knight_sound
ROM_END

ROM_START( j6knighte )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "68d8", 0x000000, 0x020000, CRC(b4809252) SHA1(cdfd32a02be11f0e546e54036e995d072bd9ca49) )
	ROM_LOAD16_BYTE( "278e", 0x000001, 0x020000, CRC(d3fe21b8) SHA1(a0813c4007b3058d0a23e3f658466e30bd91cc51) )
	j6knight_sound
ROM_END


ROM_START( j6svndb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ace.evn", 0x000000, 0x020000, CRC(0078e610) SHA1(545f3aa30154e7a75d72bc4621010177f7b1b441) )
	ROM_LOAD16_BYTE( "ace.odd", 0x000001, 0x020000, CRC(422268c4) SHA1(defaab4476aad6ad17ff917fa795a6a5a1828090) )

//  ROM_LOAD( "ace.bin", 0x000000, 0x040000, CRC(272070fd) SHA1(cd5f06106f0379b2769515193aee61dea04c1f1e) ) // this is just a combined rom of the above

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END

#define j6ewndg_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "ewnsnd.bin", 0x000000, 0x020000, CRC(6d91e98c) SHA1(4acf46758089f2519027db148665aa75789d2d68) )

ROM_START( j6ewn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ewn.p1", 0x000000, 0x020000, CRC(c1b3fa48) SHA1(fc250876b99af70562473e9d03c233dbf53a82c9) )
	ROM_LOAD16_BYTE( "ewn.p2", 0x000001, 0x020000, CRC(60f66b8d) SHA1(7ac2b741ecddd379c86a95bddcc9e0a82a5272b6) )
	//ROM_LOAD(  "ewn8_8.b16", 0x0000, 0x040000, CRC(674fd826) SHA1(34b17cdc4b8eb67c1100e62dee8bf13c66a5fa82) ) // just the above joined up

	j6ewndg_sound
ROM_END

ROM_START( j6ewndg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "ewnc1.bin", 0x00000, 0x020000, CRC(9ee88773) SHA1(4b0772ddf7061300a914fe1a47d696e50be8c04b) )
	ROM_LOAD16_BYTE(  "ewnc2.bin", 0x00001, 0x020000, CRC(791b2108) SHA1(3f8f12e2e3941d95f0332eb828a3f4be60a81742) )
	j6ewndg_sound
ROM_END

ROM_START( j6ewndga )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "ewnc1ss.bin", 0x0000, 0x020000, CRC(cc2c4712) SHA1(15729b5bccdb5f0e71bdedbe8817988c186a9d7e) )
	ROM_LOAD16_BYTE(  "ewnc2.bin", 0x00001, 0x020000, CRC(791b2108) SHA1(3f8f12e2e3941d95f0332eb828a3f4be60a81742) )
	j6ewndg_sound
ROM_END

ROM_START( j6ewndgb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "ewncp1.bin", 0x0000, 0x020000, CRC(27b2132a) SHA1(144f491eeaed2b6e0203e31fe5f78280edc41c1e) )
	ROM_LOAD16_BYTE(  "ewnc2.bin", 0x00001, 0x020000, CRC(791b2108) SHA1(3f8f12e2e3941d95f0332eb828a3f4be60a81742) )
	j6ewndg_sound
ROM_END


#define j6hikar_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */


ROM_START( j6hikar )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hk4a1.b8", 0x000000, 0x020000, CRC(de24d4d2) SHA1(9cebc0fe9476e6c555845f9ed42f13c52fd3486b) ) // aka hikarate-p1.bin
	ROM_LOAD16_BYTE( "hk4a2.b8", 0x000001, 0x020000, CRC(aaf75168) SHA1(0407464768fcd8b260926efbba3fd727df78a4f7) ) // aka hikarate-p2.bin
	j6hikar_sound
ROM_END

ROM_START( j6hikara )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hika-51.bin", 0x00000, 0x020000, CRC(533bba89) SHA1(4abfb9fc925f47e65dc45511289a966acd88c200) )
	ROM_LOAD16_BYTE( "hika-52.bin", 0x00001, 0x020000, CRC(930351ba) SHA1(40705df03e858ffbef684606ce2d667859f40e05) )
	j6hikar_sound
ROM_END

ROM_START( j6hikarb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "hika-51a.bin", 0x0000, 0x020000, CRC(01ff7ae8) SHA1(70073a34e8f0e01ceab7e8afb88c68282006080e) )
	ROM_LOAD16_BYTE( "hika-52.bin", 0x00001, 0x020000, CRC(930351ba) SHA1(40705df03e858ffbef684606ce2d667859f40e05) )
	j6hikar_sound
ROM_END



ROM_START( j6hisprt )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "spir0_1e_demo.p1", 0x000000, 0x020000, CRC(016e68db) SHA1(efb9da76b16352588ba9a831210f135b13c0fec9) )
	ROM_LOAD16_BYTE( "spir0_1e_demo.p2", 0x000001, 0x020000, CRC(49b62046) SHA1(e07db0ce27896af4f508993d935135264cfe0ba1) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "spirsnd_demo.bin", 0x000000, 0x080000, CRC(15c6771f) SHA1(a99f142f53637af361699a73e229dcce224b117f) )
ROM_END

// correct rom might be in the j6roller sets
#define j6rcclub_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6rcclub )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8478.bin", 0x000000, 0x020000, CRC(59ea4a54) SHA1(a5ec5a7e8dad51e6bed210ffacb9a7ee64046fa9) )
	ROM_LOAD16_BYTE( "8479.bin", 0x000001, 0x020000, CRC(8f45065a) SHA1(ee0ff3d4154879a5e77a438d68f6e482116fe235) )
	j6rcclub_sound
ROM_END

ROM_START( j6rccluba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8480.bin", 0x000000, 0x020000, CRC(b2741e6c) SHA1(37aee7860e7f17262c003f0fc3ba0f87ce6c376a) )
	ROM_LOAD16_BYTE( "8479.bin", 0x000001, 0x020000, CRC(8f45065a) SHA1(ee0ff3d4154879a5e77a438d68f6e482116fe235) )
	j6rcclub_sound
ROM_END

ROM_START( j6rcclubb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8481.bin", 0x000000, 0x020000, CRC(9d30a944) SHA1(76d83d29f0d83a187a1b0cc4c8c9fc3da51501c3) )
	ROM_LOAD16_BYTE( "8479.bin", 0x000001, 0x020000, CRC(8f45065a) SHA1(ee0ff3d4154879a5e77a438d68f6e482116fe235) )
	j6rcclub_sound
ROM_END


#define j6slvgst_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6slvgst )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9122.bin", 0x000000, 0x020000, CRC(6d3d9e98) SHA1(377adca43f1624b5f779132cbc833ff883531aa3) )
	ROM_LOAD16_BYTE( "9123.bin", 0x000001, 0x020000, CRC(8347b12f) SHA1(c2ed622aaf3e4de3ff8b2854eded793aed662c56) )
	j6slvgst_sound
ROM_END

ROM_START( j6slvgsta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9124.bin", 0x000000, 0x020000, CRC(86a3caa0) SHA1(793cba57e6849b4bb4b821cee26a164a2911ff9d) )
	ROM_LOAD16_BYTE( "9123.bin", 0x000001, 0x020000, CRC(8347b12f) SHA1(c2ed622aaf3e4de3ff8b2854eded793aed662c56) )
	j6slvgst_sound
ROM_END

ROM_START( j6slvgstb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9125.bin", 0x000000, 0x020000, CRC(3ff95ef9) SHA1(df4ddeace9cb37c68ff70ff6302933f2f447c6c6) )
	ROM_LOAD16_BYTE( "9123.bin", 0x000001, 0x020000, CRC(8347b12f) SHA1(c2ed622aaf3e4de3ff8b2854eded793aed662c56) )
	j6slvgst_sound
ROM_END

ROM_START( j6slvgstc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9126.bin", 0x000000, 0x020000, CRC(0f50ef10) SHA1(3bc83d627b4f54ee39fcfa3aeb2f3b75517f7f00) )
	ROM_LOAD16_BYTE( "9123.bin", 0x000001, 0x020000, CRC(8347b12f) SHA1(c2ed622aaf3e4de3ff8b2854eded793aed662c56) )
	j6slvgst_sound
ROM_END

ROM_START( j6slvgstd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9150.bin", 0x000000, 0x020000, CRC(ea1de415) SHA1(7c2844e6e166fc045aaef6b1157205d63895901c) )
	ROM_LOAD16_BYTE( "9151.bin", 0x000001, 0x020000, CRC(372fd0b0) SHA1(2f186c95d0b5dd15385c0d6404a8a105400226b5) )
	j6slvgst_sound
ROM_END

ROM_START( j6slvgste )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9152.bin", 0x000000, 0x020000, CRC(0183b02d) SHA1(d8ddb012a141697abd1a198ea991ff33f7257a23) )
	ROM_LOAD16_BYTE( "9151.bin", 0x000001, 0x020000, CRC(372fd0b0) SHA1(2f186c95d0b5dd15385c0d6404a8a105400226b5) )
	j6slvgst_sound
ROM_END

ROM_START( j6slvgstf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9153.bin", 0x000000, 0x020000, CRC(b8d92474) SHA1(1ae7eb663a620fab8e7bf2f67eeea35607f081b1) )
	ROM_LOAD16_BYTE( "9151.bin", 0x000001, 0x020000, CRC(372fd0b0) SHA1(2f186c95d0b5dd15385c0d6404a8a105400226b5) )
	j6slvgst_sound
ROM_END

ROM_START( j6slvgstg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "9154.bin", 0x000000, 0x020000, CRC(8870959d) SHA1(1d073d7ac82a1dd47473cce9aa238d23e45a90ac) )
	ROM_LOAD16_BYTE( "9151.bin", 0x000001, 0x020000, CRC(372fd0b0) SHA1(2f186c95d0b5dd15385c0d6404a8a105400226b5) )
	j6slvgst_sound
ROM_END

#define j6footy_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "ffsnd.bin", 0x000000, 0x080000, CRC(99b8f4bd) SHA1(afc8e24db67f841a570b4cdd780a759a8fa13055) )
// note: rom2 gets replaced between sets instead of rom1 (it's by Empire, not JPM)
ROM_START( j6footy )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ff1-03p1.bin", 0x000000, 0x020000, CRC(95f6f955) SHA1(1e7427d442a00f57f83fa16c8a485b110e3a01b8) )
	ROM_LOAD16_BYTE( "ff1-03p2.bin", 0x000001, 0x020000, CRC(ba2369ad) SHA1(9ba7bd782f1a287c9dc0a29e26c77fd82ad45f59) )
	j6footy_sound
ROM_END

ROM_START( j6footya )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ff1-03p1.bin", 0x000000, 0x020000, CRC(95f6f955) SHA1(1e7427d442a00f57f83fa16c8a485b110e3a01b8) )
	ROM_LOAD16_BYTE( "ff1-03i2.bin", 0x000001, 0x020000, CRC(542d0881) SHA1(e4ce44ec47ed1f5cc6c2f36b7d4ced75e3f9dfdb) )
	j6footy_sound
ROM_END

ROM_START( j6footyb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ff1-03p1.bin", 0x000000, 0x020000, CRC(95f6f955) SHA1(1e7427d442a00f57f83fa16c8a485b110e3a01b8) )
	ROM_LOAD16_BYTE( "ff1-03x2.bin", 0x000001, 0x020000, CRC(bd4eadb4) SHA1(e3875456c67c3ea813bf13421089be4383731d4a) )
	j6footy_sound
ROM_END






#define j6bmc_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "bigmoneysnd.bin", 0x0000, 0x080000, CRC(dd0a88c6) SHA1(22206fcba097a4f7dc6ae84d496d149a4206e0f0) )
ROM_START( j6bmc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "bimo-91.bin", 0x00000, 0x020000, CRC(691a15b8) SHA1(d48d80ed59b7d20a4910a5b0554e64f8fe324bef) )
	ROM_LOAD16_BYTE(  "bimo-92.bin", 0x00001, 0x020000, CRC(3a3503d2) SHA1(6e6bc2c07677b0a2416b084fb3204cc47373cc6a) )
	j6bmc_sound
ROM_END

ROM_START( j6bmca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "bm71.b8", 0x0000, 0x020000, CRC(7bcca348) SHA1(9b4ac6b48c0ed258c5fd257c452661e664662866) )
	ROM_LOAD16_BYTE(  "bm72.b8", 0x0001, 0x020000, CRC(d28d8990) SHA1(cf92b5310db7ede40dd6c0e8d3f2a11b5bcb0745) )
	j6bmc_sound
ROM_END


#define j6bno_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "bignightoutsnd.bin", 0x0000, 0x080000, CRC(e4da3e2e) SHA1(9751dfa7f9ca11b7073742b0ba2bf90fb483452b) ) /* aka bignite.as1 */
ROM_START( j6bno )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "bignightout.p1", 0x00000, 0x020000, CRC(d9a9858f) SHA1(103a31682eb28f0585e948ad8f59887ad2976f40) )
	ROM_LOAD16_BYTE(  "bignightout.p2", 0x00001, 0x020000, CRC(d17a7e13) SHA1(f40e46070a59e10cb08ec47f7ce53694ef13b311) )
	j6bno_sound
ROM_END

ROM_START( j6bnoa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "bignite.g1", 0x00000, 0x020000, CRC(b06a2513) SHA1(36012ad32a8ad824028866fdee86847a042dc8c5) )
	ROM_LOAD16_BYTE(  "bignite.g2", 0x00001, 0x020000, CRC(d929f64e) SHA1(ed027f97adee2188a342908d8224a796b6b0991b) )
	j6bno_sound
ROM_END

ROM_START( j6bnob )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "bnol1.b8", 0x00000, 0x020000, CRC(8dc17175) SHA1(7af502eb90f2d7ab14d747ef9b96f2eade70463a) )
	ROM_LOAD16_BYTE(  "bnol2.b8", 0x00001, 0x020000, CRC(2170044b) SHA1(f985bb42b84f5729ec3ce9d5b8c9ec47c6380e52) )
	j6bno_sound
ROM_END

ROM_START( j6bnoc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "bnolp.b8", 0x00000, 0x020000, CRC(665f254d) SHA1(65ea616d46664882d84b9945cca384e175a18f3f) )
	ROM_LOAD16_BYTE(  "bnol2.b8", 0x00001, 0x020000, CRC(2170044b) SHA1(f985bb42b84f5729ec3ce9d5b8c9ec47c6380e52) )
	j6bno_sound
ROM_END

#define j6btbw_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "borntobewildsnd.bin", 0x0000, 0x080000, CRC(2c590926) SHA1(d5f56624d1f8f9692004937f98cadde78c2606bc) )

ROM_START( j6btbw )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "a7df", 0x00000, 0x020000, CRC(9620c56e) SHA1(be1818cb5f5c9ed88fa250e7470ca18724a0bc0b) )
	ROM_LOAD16_BYTE(  "2c84", 0x00001, 0x020000, CRC(37e242d4) SHA1(66c7327272ef469c44937137e86406edf0ebd5e1) )
	j6btbw_sound
ROM_END

ROM_START( j6btbwa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "a7f7", 0x0000, 0x020000, CRC(63cc9eba) SHA1(32396f0f64fbcfbf3e824b1557ff8b34c5f1e041) )
	ROM_LOAD16_BYTE(  "2c84", 0x00001, 0x020000, CRC(37e242d4) SHA1(66c7327272ef469c44937137e86406edf0ebd5e1) )
	j6btbw_sound
ROM_END

ROM_START( j6btbwb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "a7fb", 0x0000, 0x020000, CRC(4a617a1c) SHA1(14cbb644ffd8b28fb1856e0e61133511a7a91671) )
	ROM_LOAD16_BYTE(  "2c84", 0x00001, 0x020000, CRC(37e242d4) SHA1(66c7327272ef469c44937137e86406edf0ebd5e1) )
	j6btbw_sound
ROM_END

ROM_START( j6btbwc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "a7fe", 0x0000, 0x020000, CRC(b9647246) SHA1(daad9b7a3b2ab9899671c07ee2cd2af106fe612c) )
	ROM_LOAD16_BYTE(  "2c84", 0x00001, 0x020000, CRC(37e242d4) SHA1(66c7327272ef469c44937137e86406edf0ebd5e1) )
	j6btbw_sound
ROM_END

ROM_START( j6btbwd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "a7ff", 0x0000, 0x020000, CRC(52fa267e) SHA1(b9f63e50a0b2fe39c3ce43914c05136e11536200) )
	ROM_LOAD16_BYTE(  "2c84", 0x00001, 0x020000, CRC(37e242d4) SHA1(66c7327272ef469c44937137e86406edf0ebd5e1) )
	j6btbw_sound
ROM_END



#define j6cpal_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD( "cpalace22-06-98 68d0a91.bin", 0x0000, 0x080000, CRC(56d581e0) SHA1(d22796ec6d96f4d4ea10dfdb925ceaff032fe7d0) ) \
	ROM_LOAD( "capa-snd.bin", 0x0000, 0x080000, CRC(56d581e0) SHA1(d22796ec6d96f4d4ea10dfdb925ceaff032fe7d0) )

ROM_START( j6cpal )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "capa1.bin", 0x00000, 0x020000, CRC(ee4221d1) SHA1(d79440ddbb7083c95d15a00e3e9395b43a1af376) )
	ROM_LOAD16_BYTE( "capa2.bin", 0x00001, 0x020000, CRC(775f6d12) SHA1(03559f8f7830b84cb718e0d005d842ddf515b6a1) )
	j6cpal_sound
ROM_END

ROM_START( j6cpala )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "capa-7p1.bin", 0x0000, 0x020000, CRC(5718b588) SHA1(2233a9848c7520e3158e4b69d2253a5c023e7015) )
	ROM_LOAD16_BYTE( "capa2.bin", 0x00001, 0x020000, CRC(775f6d12) SHA1(03559f8f7830b84cb718e0d005d842ddf515b6a1) )
	j6cpal_sound
ROM_END

ROM_START( j6cpalb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "capa-7s1.bin", 0x0000, 0x020000, CRC(bc86e1b0) SHA1(51a00a30fb9fac86f2790fb6904298561f8f4183) )
	ROM_LOAD16_BYTE( "capa2.bin", 0x00001, 0x020000, CRC(775f6d12) SHA1(03559f8f7830b84cb718e0d005d842ddf515b6a1) )
	j6cpal_sound
ROM_END

ROM_START( j6cpalc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "capa-7w1.bin", 0x0000, 0x020000, CRC(deeb9038) SHA1(167ad8e3984c13791db15b53194fa0e3b5f79b76) )
	ROM_LOAD16_BYTE( "capa2.bin", 0x00001, 0x020000, CRC(775f6d12) SHA1(03559f8f7830b84cb718e0d005d842ddf515b6a1) )
	j6cpal_sound
ROM_END

ROM_START( j6cpald )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "capa-b1.bin", 0x00000, 0x020000, CRC(03855437) SHA1(92a6f9feeed7b0a0c1c420561bc74892ec0a5c89) )
	ROM_LOAD16_BYTE( "capa-b2.bin", 0x00001, 0x020000, CRC(74b590bc) SHA1(0c11b97b15eee082133340bc685e4257d769b88f) )
	j6cpal_sound
ROM_END

ROM_START( j6cpale )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "cpa1.bin", 0x00000, 0x020000, CRC(84f4c191) SHA1(bc5466b03818b653e7286ab95cfda0d2e9f64251) )
	ROM_LOAD16_BYTE(  "cpa2.bin", 0x00001, 0x020000, CRC(9b0ca91a) SHA1(86e49c02966ee62e030d526a6329d3be715fb3db) )
	j6cpal_sound
ROM_END

ROM_START( j6cpalf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "cpa1p.bin", 0x0000, 0x020000, CRC(6f6a95a9) SHA1(96d54598abcf887afeb1c878c6b4acbce7d24039) )
	ROM_LOAD16_BYTE(  "cpa2.bin", 0x00001, 0x020000, CRC(9b0ca91a) SHA1(86e49c02966ee62e030d526a6329d3be715fb3db) )
	j6cpal_sound
ROM_END

ROM_START( j6cpalg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ceas_pal.p1", 0x00000, 0x020000, CRC(e8df4b09) SHA1(c04fcede76eff727aed979cd7bed5401be2480e3) )
	ROM_LOAD16_BYTE( "ceas_pal.p2", 0x00001, 0x020000, CRC(4569254c) SHA1(f058dd5b97bd94e59fd952f42696f1f0aaede016) )
	j6cpal_sound
ROM_END


ROM_START( j6cdivr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fsd.p1", 0x00000, 0x020000, CRC(e9266e02) SHA1(407b99a329b91864d6552d4ae2e13d8a5880930b) )
	ROM_LOAD16_BYTE(  "fsd.p2", 0x00001, 0x020000, CRC(9be40086) SHA1(7b24c3e42299f09a2ad66785673a28ff326537cf) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END



#define j6colic_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6colic )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "coli-11.bin", 0x00000, 0x020000, CRC(1dbfc333) SHA1(264b2ad65d58ad7c6b569d2896af69973709d5dc) )
	ROM_LOAD16_BYTE(  "coli-12.bin", 0x00001, 0x020000, CRC(1d938c67) SHA1(1a6e7a472d55f17f92d29bd99fcd6a753eb856d9) )
	j6colic_sound
ROM_END

ROM_START( j6colica )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "coli-1a1.bin", 0x0000, 0x020000, CRC(4f7b0352) SHA1(b98791c5af9262e7a389525178b4270146a9a825) )
	ROM_LOAD16_BYTE(  "coli-12.bin", 0x00001, 0x020000, CRC(1d938c67) SHA1(1a6e7a472d55f17f92d29bd99fcd6a753eb856d9) )
	j6colic_sound
ROM_END

ROM_START( j6colicb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "coli-1n1.bin", 0x0000, 0x020000, CRC(2c897bf7) SHA1(b7418506de2600b1eeb155849cce821eba629d74) )
	ROM_LOAD16_BYTE(  "coli-12.bin", 0x00001, 0x020000, CRC(1d938c67) SHA1(1a6e7a472d55f17f92d29bd99fcd6a753eb856d9) )
	j6colic_sound
ROM_END

ROM_START( j6colicc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "coli-1p1.bin", 0x0000, 0x020000, CRC(f621970b) SHA1(e772093135321e256483dd4a6e72a1366d7e28c0) )
	ROM_LOAD16_BYTE(  "coli-12.bin", 0x00001, 0x020000, CRC(1d938c67) SHA1(1a6e7a472d55f17f92d29bd99fcd6a753eb856d9) )
	j6colic_sound
ROM_END

ROM_START( j6colicd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "coli1np1.bin", 0x0000, 0x020000, CRC(c7172fcf) SHA1(fee2f9023b651e4c2a799f413060536a775f43c5) )
	ROM_LOAD16_BYTE(  "coli-12.bin", 0x00001, 0x020000, CRC(1d938c67) SHA1(1a6e7a472d55f17f92d29bd99fcd6a753eb856d9) )
	j6colic_sound
ROM_END



#define j6crakr_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */
ROM_START( j6crakr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "crac-11.bin", 0x00000, 0x020000, CRC(facb09cd) SHA1(3f70f3616a2201d25d3712012b4c56cfa09c1411) )
	ROM_LOAD16_BYTE(  "crac-12.bin", 0x00001, 0x020000, CRC(16472d00) SHA1(cea574e7c8e42b8832f9849899e56d9b73d97ed1) )
	j6crakr_sound
ROM_END

ROM_START( j6crakra )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "crac-1a1.bin", 0x0000, 0x020000, CRC(a80fc9ac) SHA1(2ad7d1e18c1ad1560039cda63014d17a5151379f) )
	ROM_LOAD16_BYTE(  "crac-12.bin", 0x00001, 0x020000, CRC(16472d00) SHA1(cea574e7c8e42b8832f9849899e56d9b73d97ed1) )
	j6crakr_sound
ROM_END

ROM_START( j6crakrb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "crac-1p1.bin", 0x0000, 0x020000, CRC(11555df5) SHA1(7a1bb63b40cb3169bf0f4c45a032beba911f2d30) )
	ROM_LOAD16_BYTE(  "crac-12.bin", 0x00001, 0x020000, CRC(16472d00) SHA1(cea574e7c8e42b8832f9849899e56d9b73d97ed1) )
	j6crakr_sound
ROM_END




#define j6easy_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "easymoneysnd.bin", 0x0000, 0x080000, CRC(aee5bc30) SHA1(071038e7fc7767b7b11c9b97b41e079fbbe11291) )

ROM_START( j6easy )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "emb1.b8", 0x00000, 0x020000, CRC(edeab2f6) SHA1(5fa20c2323049a1f92284fb331d836de7fc907c5) )
	ROM_LOAD16_BYTE(  "emb2.b8", 0x00001, 0x020000, CRC(b8344d42) SHA1(864d28960112095f23b62648e296ea068cd824ce) )
	j6easy_sound
ROM_END

ROM_START( j6easya )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "embp1.b8", 0x0000, 0x020000, CRC(0674e6ce) SHA1(698130fae4a40f289407ac56a221c77a493ee16b) )
	ROM_LOAD16_BYTE(  "emb2.b8", 0x00001, 0x020000, CRC(b8344d42) SHA1(864d28960112095f23b62648e296ea068cd824ce) )
	j6easy_sound
ROM_END

ROM_START( j6easyb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "emc1.b8", 0x0000, 0x020000, CRC(95a1c269) SHA1(d0502c3408a285523f718de54df8ef1a265a0221) )
	ROM_LOAD16_BYTE(  "emc2.b8", 0x0001, 0x020000, CRC(5b4ecf73) SHA1(327b9a517e1a2c22e1da6c10fb32eb16bc182d5c) )
	j6easy_sound
ROM_END

ROM_START( j6easyc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "emcp.b8", 0x0000, 0x020000, CRC(7e3f9651) SHA1(ad14b3b7a14c1b86fae94f44f3dfc52cfbfd136f) )
	ROM_LOAD16_BYTE(  "emc2.b8", 0x0001, 0x020000, CRC(5b4ecf73) SHA1(327b9a517e1a2c22e1da6c10fb32eb16bc182d5c) )
	j6easy_sound
ROM_END


#define j6ffc_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6ffc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "eedf", 0x00000, 0x020000, CRC(cba081fa) SHA1(bdb547c2922e9b8bccd9c12cc6aa8b12ef4bc3b2) )
	ROM_LOAD16_BYTE(  "c0f2", 0x00001, 0x020000, CRC(fb68bbba) SHA1(4978d305409d5f024ed2ef18b32aa5cc582ce83d) )
	j6ffc_sound
ROM_END

ROM_START( j6ffca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "eef7", 0x0000, 0x020000, CRC(3e4cda2e) SHA1(e5d2d57d114230356afd50e0a854449c30b84bf3) )
	ROM_LOAD16_BYTE(  "c0f2", 0x00001, 0x020000, CRC(fb68bbba) SHA1(4978d305409d5f024ed2ef18b32aa5cc582ce83d) )
	j6ffc_sound
ROM_END

ROM_START( j6ffcb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "eefb", 0x0000, 0x020000, CRC(17e13e88) SHA1(9f51f0d32370e5bb9d3efe60c6a83dcffaff586b) )
	ROM_LOAD16_BYTE(  "c0f2", 0x00001, 0x020000, CRC(fb68bbba) SHA1(4978d305409d5f024ed2ef18b32aa5cc582ce83d) )
	j6ffc_sound
ROM_END

ROM_START( j6ffcc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "eefe", 0x0000, 0x020000, CRC(e4e436d2) SHA1(e8c3bf1ef1737bb4bc59a7e1c2be33acec9db56b) )
	ROM_LOAD16_BYTE(  "c0f2", 0x00001, 0x020000, CRC(fb68bbba) SHA1(4978d305409d5f024ed2ef18b32aa5cc582ce83d) )
	j6ffc_sound
ROM_END

ROM_START( j6ffcd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "eeff", 0x0000, 0x020000, CRC(0f7a62ea) SHA1(9867488b8ceeb24b5c7c7f5824603f6310b1b97d) )
	ROM_LOAD16_BYTE(  "c0f2", 0x00001, 0x020000, CRC(fb68bbba) SHA1(4978d305409d5f024ed2ef18b32aa5cc582ce83d) )
	j6ffc_sound
ROM_END

ROM_START( j6ffce )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "frfo-d1.bin", 0x0000, 0x020000, CRC(711ac380) SHA1(d85298372fc6fd473fc84038c3c05733436bbb9f) )
	ROM_LOAD16_BYTE(  "frfo-d2.bin", 0x0001, 0x020000, CRC(1744b355) SHA1(1c5f07f1b43ff189e9618bf6012485cab7894712) )
	j6ffc_sound
ROM_END

#define j6grc_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */


ROM_START( j6grc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fqb1.bin", 0x00000, 0x020000, CRC(e479c554) SHA1(397a36e12ebe47d67ad209c66a2da302191e16e2) )
	ROM_LOAD16_BYTE(  "fqb2.bin", 0x00001, 0x020000, CRC(19c653df) SHA1(f5fd7bc0d68eadfa241a01d5c5d7bd91642e0376) )
	j6grc_sound
ROM_END

ROM_START( j6grca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fqbp1.bin", 0x0000, 0x020000, CRC(0fe7916c) SHA1(f251f5fd07ceeb7aa9e09cb95a07f28f80cd59ce) )
	ROM_LOAD16_BYTE(  "fqb2.bin", 0x00001, 0x020000, CRC(19c653df) SHA1(f5fd7bc0d68eadfa241a01d5c5d7bd91642e0376) )
	j6grc_sound
ROM_END


#define j6hdc_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "club-hotdogssnd.bin", 0x0000, 0x080000, CRC(2bd7871f) SHA1(dc129f64f7186c02f4283229b579275ecb3a1165) )

ROM_START( j6hdc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "club-hotdogs.p1", 0x00000, 0x020000, CRC(791a59d5) SHA1(5653638d9c138471afcbee9b0e81246d76d7d57f) )
	ROM_LOAD16_BYTE(  "club-hotdogs.p2", 0x00001, 0x020000, CRC(4b993b62) SHA1(dd0d6f289bbbc28b83a803be822293f2d2125347) )
	j6hdc_sound
ROM_END

ROM_START( j6hdca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "chd4d_1f", 0x0000, 0x020000, CRC(bdc0bac5) SHA1(875336aac8a3b456aa24e2f683617d76c415d0ea) )
	ROM_LOAD16_BYTE(  "club-hotdogs.p2", 0x00001, 0x020000, CRC(4b993b62) SHA1(dd0d6f289bbbc28b83a803be822293f2d2125347) )
	j6hdc_sound
ROM_END

ROM_START( j6hdcb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "chd4d_1j", 0x0000, 0x020000, CRC(482ce111) SHA1(0d82973f8123b2174d8d2df9ca7f3929da449dd3) )
	ROM_LOAD16_BYTE(  "club-hotdogs.p2", 0x00001, 0x020000, CRC(4b993b62) SHA1(dd0d6f289bbbc28b83a803be822293f2d2125347) )
	j6hdc_sound
ROM_END

ROM_START( j6hdcc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "chd4d_1p", 0x0000, 0x020000, CRC(92840ded) SHA1(31b4161226a771747b488ebd81e9539928ae5dcb) )
	ROM_LOAD16_BYTE(  "club-hotdogs.p2", 0x00001, 0x020000, CRC(4b993b62) SHA1(dd0d6f289bbbc28b83a803be822293f2d2125347) )
	j6hdc_sound
ROM_END

ROM_START( j6hdcd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "chd4d_1s", 0x0000, 0x020000, CRC(dc93d917) SHA1(1f4f0c33fa6bfd5f4070a154983447e8e5c75fa9) )
	ROM_LOAD16_BYTE(  "club-hotdogs.p2", 0x00001, 0x020000, CRC(4b993b62) SHA1(dd0d6f289bbbc28b83a803be822293f2d2125347) )
	j6hdc_sound
ROM_END

ROM_START( j6hdce )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "c9f7", 0x0000, 0x020000, CRC(067976c6) SHA1(cf18b659ed6c372b5a61f6aca991f23897144953) )
	ROM_LOAD16_BYTE(  "29a7", 0x00001, 0x020000, CRC(23b61c92) SHA1(f0fb308f93ec3534ce583da5af5e772a219a0505) )
	j6hdc_sound
ROM_END

ROM_START( j6hdcf )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "c9fe", 0x0000, 0x020000, CRC(dcd19a3a) SHA1(5d39070d9d8dbb0c1f6c544ea51a0f8428fc50c4) )
	ROM_LOAD16_BYTE(  "29a7", 0x00001, 0x020000, CRC(23b61c92) SHA1(f0fb308f93ec3534ce583da5af5e772a219a0505) )
	j6hdc_sound
ROM_END

ROM_START( j6hdcg )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "c9ff", 0x0000, 0x020000, CRC(374fce02) SHA1(4e5d7d452f6345388a5989d0059344000c691161) )
	ROM_LOAD16_BYTE(  "29a7", 0x00001, 0x020000, CRC(23b61c92) SHA1(f0fb308f93ec3534ce583da5af5e772a219a0505) )
	j6hdc_sound
ROM_END




ROM_START( j6impls )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "impc1.b8", 0x00000, 0x020000, CRC(b4b75883) SHA1(95e5e87df6eca95d3317a2b0a4ab487c6ec2d0cb) )
	ROM_LOAD16_BYTE(  "impc2.b8", 0x00001, 0x020000, CRC(0cffbc0d) SHA1(c8f4dc280526cb2ab23c6fe82939c1c228948acf) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END


#define j6kapang_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */
ROM_START( j6kapang )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "kapa-41.bin", 0x00000, 0x020000, CRC(34509f12) SHA1(b7de8c6004b9638365dceed79d9a829587c45ab4) )
	ROM_LOAD16_BYTE(  "kapa-42.bin", 0x00001, 0x020000, CRC(5d03b7eb) SHA1(a5e7a0674eb0d5798cdcd467f76aed92d15f9df8) )
	j6kapang_sound
ROM_END

ROM_START( j6kapanga )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "kapa-4p1.bin", 0x0000, 0x020000, CRC(dfcecb2a) SHA1(fd8930a1ec80159358acf0a4ea65fd0addaa9d23) )
	ROM_LOAD16_BYTE(  "kapa-42.bin", 0x00001, 0x020000, CRC(5d03b7eb) SHA1(a5e7a0674eb0d5798cdcd467f76aed92d15f9df8) )
	j6kapang_sound
ROM_END


#define j6kfc_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "kungfuclubsound.bin", 0x0000, 0x080000, CRC(659dfb1a) SHA1(0094cdee97c82a05358e8fcc6157f761c51c3655) )
ROM_START( j6kfc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "kfcl-e1.bin", 0x00000, 0x020000, CRC(f16857a0) SHA1(5cff87bf3857593c7d032315b7555e16c3849d08) )
	ROM_LOAD16_BYTE(  "kfcl-e2.bin", 0x00001, 0x020000, CRC(9da62c89) SHA1(24d35a4c4e45d0a9bd6d0557c373b7d63f6a83aa) )
	j6kfc_sound
ROM_END

ROM_START( j6kfca )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "kfcl-ep1.bin", 0x0000, 0x020000, CRC(1af60398) SHA1(45b26b983f82298c9cb14eeb23ea30f24b8ab0e7) )
	ROM_LOAD16_BYTE(  "kfcl-e2.bin", 0x00001, 0x020000, CRC(9da62c89) SHA1(24d35a4c4e45d0a9bd6d0557c373b7d63f6a83aa) )
	j6kfc_sound
ROM_END

ROM_START( j6kfcb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "kfcl-f1.bin", 0x00000, 0x020000, CRC(4edfa088) SHA1(e23d18c94c19edaadb73e31f0526c4a3f40c5e7e) )
	ROM_LOAD16_BYTE( "kfcl-f2.bin", 0x00001, 0x020000, CRC(88d37b10) SHA1(8954e7f4a7401dc93f9f4e16d0ae9ff6d2b911d8) )
	j6kfc_sound
ROM_END



#define j6lucklo_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6lucklo )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "llb1.bin", 0x00000, 0x020000, CRC(e63a2c97) SHA1(b9e02de1c82d761209ee185b29bd248785e07cbe) )
	ROM_LOAD16_BYTE(  "llb2.bin", 0x00001, 0x020000, CRC(0d6de39d) SHA1(eb696d7cbecef4033a7f539430b9e970a82e757c) )
	j6lucklo_sound
ROM_END

ROM_START( j6luckloa )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "llbp1.bin", 0x0000, 0x020000, CRC(0da478af) SHA1(76a7f84eea5fdde8c08d5f56a612097235c4d1b8) )
	ROM_LOAD16_BYTE(  "llb2.bin", 0x00001, 0x020000, CRC(0d6de39d) SHA1(eb696d7cbecef4033a7f539430b9e970a82e757c) )
	j6lucklo_sound
ROM_END


#define j6monst_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "bad1snd.bin", 0x0000, 0x080000, CRC(1b61dcf9) SHA1(3ba4c8d9b77c86fbb931af0c0d9808ac68d0aa25) )
ROM_START( j6monst )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "monster.p1", 0x00000, 0x020000, CRC(979ba29d) SHA1(44258fc5c41794c84d0b034cc5bc789d2365f641) )
	ROM_LOAD16_BYTE(  "monster.p2", 0x00001, 0x020000, CRC(5871a333) SHA1(eb2e1dbd8c2acc7717a3178c6fe20bcde34cf2ff) )
	j6monst_sound
ROM_END

ROM_START( j6monsta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "f57f", 0x0000, 0x020000, CRC(3212225f) SHA1(7b76db4cf9cb0a656cbc43671bf453c9834b71c5) )
	ROM_LOAD16_BYTE(  "monster.p2", 0x00001, 0x020000, CRC(5871a333) SHA1(eb2e1dbd8c2acc7717a3178c6fe20bcde34cf2ff) )
	j6monst_sound
ROM_END

ROM_START( j6monstb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "f5df", 0x0000, 0x020000, CRC(5341418d) SHA1(fc744d7508b4d0d67748bc135733244eec623758) )
	ROM_LOAD16_BYTE(  "monster.p2", 0x00001, 0x020000, CRC(5871a333) SHA1(eb2e1dbd8c2acc7717a3178c6fe20bcde34cf2ff) )
	j6monst_sound
ROM_END

ROM_START( j6monstc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "f5f7", 0x0000, 0x020000, CRC(a6ad1a59) SHA1(df7f71f73d0c0dd318df17c6ec3b3788dbcfe488) )
	ROM_LOAD16_BYTE(  "monster.p2", 0x00001, 0x020000, CRC(5871a333) SHA1(eb2e1dbd8c2acc7717a3178c6fe20bcde34cf2ff) )
	j6monst_sound
ROM_END

ROM_START( j6monstd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "f5fe", 0x0000, 0x020000, CRC(7c05f6a5) SHA1(96b5c0c21119a343a94c608690766fdf8f0fc300) )
	ROM_LOAD16_BYTE(  "monster.p2", 0x00001, 0x020000, CRC(5871a333) SHA1(eb2e1dbd8c2acc7717a3178c6fe20bcde34cf2ff) )
	j6monst_sound
ROM_END






ROM_START( j6pinfvr )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "pife-31.bin", 0x00000, 0x020000, CRC(0ce84585) SHA1(ef5781f1bed3e47169a7f10145557906b2b401b1) )
	ROM_LOAD16_BYTE(  "pife-32.bin", 0x00001, 0x020000, CRC(b5e9a22c) SHA1(4694a30a11d18fedb526085353e6f784cc3ec3f9) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD(  "pfversnd.bin", 0x0000, 0x080000, CRC(27ab8a98) SHA1(afaf064d31a080a5a4c03e6141e062e33ec23353) )
ROM_END








#define j6ramese_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "ramesesrichessnd.bin", 0x0000, 0x080000, CRC(0173169e) SHA1(ccba7f6d41193f556af8ef6c827b482277ee0ee2) )
ROM_START( j6ramese )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rari-b1.bin", 0x00000, 0x020000, CRC(a9d6f938) SHA1(ab0f9fb178708ff8660f37495f1c2579c9370b27) )
	ROM_LOAD16_BYTE(  "rari-b2.bin", 0x00001, 0x020000, CRC(99483b27) SHA1(734c68d6dab75cf2c8aae7e943f4feb49c592918) )
	j6ramese_sound
ROM_END

ROM_START( j6ramesea )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rari-bp1.bin", 0x0000, 0x020000, CRC(4248ad00) SHA1(a5c0c3e924f52f1b6e9a072dc5c7f5de288b44f6) )
	ROM_LOAD16_BYTE(  "rari-b2.bin", 0x00001, 0x020000, CRC(99483b27) SHA1(734c68d6dab75cf2c8aae7e943f4feb49c592918) )
	j6ramese_sound
ROM_END

ROM_START( j6rameseb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rari-a1.bin", 0x0000, 0x020000, CRC(b7f68ceb) SHA1(29bf459564c1f33ea4987c6d2e979489f0837a9c) )
	ROM_LOAD16_BYTE(  "rari-a2.bin", 0x0001, 0x020000, CRC(2d50dc66) SHA1(5511b12d1cb1f5e28b1cbb74981f57fe9f955a60) )
	j6ramese_sound
ROM_END

ROM_START( j6ramesec )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "7fdf", 0x0000, 0x020000, CRC(df11398e) SHA1(ea26644faec4ee3138af5b784c31c3548792899c) )
	ROM_LOAD16_BYTE(  "73ae", 0x0001, 0x020000, CRC(74cc2c21) SHA1(94827e336d273207e8550ca775be7fb086a39566) )
	j6ramese_sound
ROM_END

ROM_START( j6ramesed )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "7ffb", 0x0000, 0x020000, CRC(035086fc) SHA1(e5d0457048e6866f743054efefdac03d9cd3620b) )
	ROM_LOAD16_BYTE(  "73ae", 0x0001, 0x020000, CRC(74cc2c21) SHA1(94827e336d273207e8550ca775be7fb086a39566) )
	j6ramese_sound
ROM_END

ROM_START( j6ramesee )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "7ffe", 0x0000, 0x020000, CRC(f0558ea6) SHA1(2fe1918200126faae7bcfdb9ff8722ccc3fe4f57) )
	ROM_LOAD16_BYTE(  "73ae", 0x0001, 0x020000, CRC(74cc2c21) SHA1(94827e336d273207e8550ca775be7fb086a39566) )
	j6ramese_sound
ROM_END

ROM_START( j6ramesef )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "7fff", 0x0000, 0x020000, CRC(1bcbda9e) SHA1(916945650feacfd5c476d01837dc78efafde3c75) )
	ROM_LOAD16_BYTE(  "73ae", 0x0001, 0x020000, CRC(74cc2c21) SHA1(94827e336d273207e8550ca775be7fb086a39566) )
	j6ramese_sound
ROM_END



#define j6r2rum_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "rtrsnd.bin", 0x0000, 0x080000, CRC(ba72e377) SHA1(99e123eebb8e7ceb2fb36fd17f1c23d3ce04d2d6) )

ROM_START( j6r2rum )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rtr-11.bin", 0x00000, 0x020000, CRC(979246a1) SHA1(78666f916bced21a6057f2c96fad8dde70df14bd) )
	ROM_LOAD16_BYTE(  "rtr-12.bin", 0x00001, 0x020000, CRC(1bc680a3) SHA1(43cc3e3b24de205345350ae6ac773969f698ae09) )
	j6r2rum_sound
ROM_END

ROM_START( j6r2ruma )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rtr-1a1.bin", 0x0000, 0x020000, CRC(c55686c0) SHA1(48d4a24585a6d5735c07f9df16270d12b6f74030) )
	ROM_LOAD16_BYTE(  "rtr-12.bin", 0x00001, 0x020000, CRC(1bc680a3) SHA1(43cc3e3b24de205345350ae6ac773969f698ae09) )
	j6r2rum_sound
ROM_END

ROM_START( j6r2rumb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rtr-1p1.bin", 0x0000, 0x020000, CRC(7c0c1299) SHA1(78f96af3f8c91c14f4341c74d8b89b7486bfaa56) )
	ROM_LOAD16_BYTE(  "rtr-12.bin", 0x00001, 0x020000, CRC(1bc680a3) SHA1(43cc3e3b24de205345350ae6ac773969f698ae09) )
	j6r2rum_sound
ROM_END

ROM_START( j6r2rumc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rrb1.b8", 0x00000, 0x020000, CRC(0970162e) SHA1(5e83f7b6ca1e2b33a5adbbb8ad69c60c213d2ed3) )
	ROM_LOAD16_BYTE(  "rrb2.b8", 0x00001, 0x020000, CRC(17d03879) SHA1(d5d569435a3e131f71dcd4e4dd56c227801fe898) )
	j6r2rum_sound
ROM_END

ROM_START( j6r2rumd )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rrba.b8", 0x0000, 0x020000, CRC(5bb4d64f) SHA1(6b61181a73dc08a9e7fe6206a339dedd311505cf) )
	ROM_LOAD16_BYTE(  "rrb2.b8", 0x00001, 0x020000, CRC(17d03879) SHA1(d5d569435a3e131f71dcd4e4dd56c227801fe898) )
	j6r2rum_sound
ROM_END

ROM_START( j6r2rume )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "rrbp.b8", 0x0000, 0x020000, CRC(e2ee4216) SHA1(d9a29b04ca6018bfd03bb09c2391d69a1a27e271) )
	ROM_LOAD16_BYTE(  "rrb2.b8", 0x00001, 0x020000, CRC(17d03879) SHA1(d5d569435a3e131f71dcd4e4dd56c227801fe898) )
	j6r2rum_sound
ROM_END


#define j6slagn_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "snlsasnd.bin", 0x0000, 0x080000, CRC(288e4ce1) SHA1(6690eccb1af94731af160d73b4d8903a98fa27bb) )
ROM_START( j6slagn )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "s+l31.b8", 0x00000, 0x020000, CRC(e8cadadb) SHA1(e7edfa79e1648658e200ddbbb1d280641e03b93e) )
	ROM_LOAD16_BYTE(  "s+l32.b8", 0x00001, 0x020000, CRC(b2251a43) SHA1(024c82509026561022ebcd3b2654d5beaa838d24) )
	j6slagn_sound
ROM_END

ROM_START( j6slagna )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "s+l3p.b8", 0x0000, 0x020000, CRC(03548ee3) SHA1(f4727303725b005ae04d9e4572a9249c07b705f3) )
	ROM_LOAD16_BYTE(  "s+l32.b8", 0x00001, 0x020000, CRC(b2251a43) SHA1(024c82509026561022ebcd3b2654d5beaa838d24) )
	j6slagn_sound
ROM_END

ROM_START( j6slagnb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "snla-41.bin", 0x0000, 0x020000, CRC(bb964fe0) SHA1(451dd54d9bb9e066dd650551861f3ee818f81d7c) )
	ROM_LOAD16_BYTE(  "snla-42.bin", 0x0001, 0x020000, CRC(7af3cc94) SHA1(34111cff7dcf61c5d3c14f58456d88e7eda2e0c1) )
	j6slagn_sound
ROM_END

ROM_START( j6slagnc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "snla-4p1.bin", 0x0000, 0x020000, CRC(50081bd8) SHA1(ebbf1d1b4c7ec03667ff617ee83065764d3f89db) )
	ROM_LOAD16_BYTE(  "snla-42.bin", 0x0001, 0x020000, CRC(7af3cc94) SHA1(34111cff7dcf61c5d3c14f58456d88e7eda2e0c1) )
	j6slagn_sound
ROM_END







#define j6thril_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "thlr-snd.bin", 0x0000, 0x020000, CRC(03771692) SHA1(2f42859d09e4354689887f1e40f9fce1eb858e3e) ) /* probably bad */ \
	ROM_LOAD(  "thrillersnd.bin", 0x0000, 0x080000, CRC(c93bc625) SHA1(ecceddd90a721c39ef3da8cef442c0d78dacaed2) )
ROM_START( j6thril )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fpe1.bin", 0x00000, 0x020000, CRC(d2e60e6f) SHA1(525022907858021a2a274a870336de6b414c0b05) )
	ROM_LOAD16_BYTE(  "fpe2.bin", 0x00001, 0x020000, CRC(08700f25) SHA1(aa1b80a7805a80899c3a852866e93fbce9b0ad7c) )
	j6thril_sound
ROM_END

ROM_START( j6thrila )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "fpep1.bin", 0x0000, 0x020000, CRC(39785a57) SHA1(df748f3c92584c8549d4e60cb61f3deadf9a516e) )
	ROM_LOAD16_BYTE(  "fpe2.bin", 0x00001, 0x020000, CRC(08700f25) SHA1(aa1b80a7805a80899c3a852866e93fbce9b0ad7c) )
	j6thril_sound
ROM_END

#define j6tqust_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	ROM_LOAD(  "tresquestsnd.bin", 0x0000, 0x080000, CRC(c7254d62) SHA1(9b97c7d1a844cd39c3dcfd984e1908ece47db00e) )
ROM_START( j6tqust )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "tq21.b8", 0x00000, 0x020000, CRC(b114c904) SHA1(bf4d2f13525aa9b91bd5985eb62e022751bba596) )
	ROM_LOAD16_BYTE(  "tq22.b8", 0x00001, 0x020000, CRC(aa62d195) SHA1(a9ba505245580444d03c7aa5f6aa3b54cb6b6fbc) )
	j6tqust_sound
ROM_END

ROM_START( j6tqusta )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "tq2p.b8", 0x0000, 0x020000, CRC(5a8a9d3c) SHA1(cee5dec688f12ea0c7477c12493324e2f0162a6a) )
	ROM_LOAD16_BYTE(  "tq22.b8", 0x00001, 0x020000, CRC(aa62d195) SHA1(a9ba505245580444d03c7aa5f6aa3b54cb6b6fbc) )
	j6tqust_sound
ROM_END

ROM_START( j6tqustb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "trqu-31.bin", 0x0000, 0x020000, CRC(d6fe4bdc) SHA1(ee95e7f1bc5f48691f1f0dfdb57a9ea38c58c557) )
	ROM_LOAD16_BYTE(  "trqu-32.bin", 0x0001, 0x020000, CRC(4fabc6ba) SHA1(eb643359e687c5b33847b782b0d3e5b33c891fca) )
	j6tqust_sound
ROM_END

ROM_START( j6tqustc )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE(  "trqu-3p1.bin", 0x0000, 0x020000, CRC(3d601fe4) SHA1(84b3c53165e4e416173861f9bf9f1be1c74888a4) )
	ROM_LOAD16_BYTE(  "trqu-32.bin", 0x0001, 0x020000, CRC(4fabc6ba) SHA1(eb643359e687c5b33847b782b0d3e5b33c891fca) )
	j6tqust_sound
ROM_END




ROM_START( j6fbcrz )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "fc.p1", 0x00000, 0x020000, CRC(6493c4c6) SHA1(ae4c18b35a55ef08b02fdabb8021eb008660543e) )
	ROM_LOAD16_BYTE( "fc.p2", 0x00001, 0x020000, CRC(2563c918) SHA1(7b5a58892019d59aff02f982cafd8617f116a812) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	ROM_LOAD( "fcsnd.bin", 0x0000, 0x080000, CRC(53806516) SHA1(e5689759ccba30ac974eee4361330ad503a29909) )
ROM_END

#define j6h5clb_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6h5clb )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8872.bin", 0x00000, 0x020000, CRC(3a3be2e9) SHA1(9078fd612cb5f195f0d9bddc5f04e1389b4f7233) )
	ROM_LOAD16_BYTE( "8873.bin", 0x00001, 0x020000, CRC(6fbe7ca4) SHA1(d7fbd1d83f165f899c5ae8ec3243ec3d8be5563d) )
	j6h5clb_sound
ROM_END

ROM_START( j6h5clba )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "8874.bin", 0x0000, 0x020000, CRC(d1a5b6d1) SHA1(54300e459cc3ea756528424f08da5505f629abd4) )
	ROM_LOAD16_BYTE( "8873.bin", 0x00001, 0x020000, CRC(6fbe7ca4) SHA1(d7fbd1d83f165f899c5ae8ec3243ec3d8be5563d) )
	j6h5clb_sound
ROM_END

#define j6milln_sound \
	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 ) \
	/* missing? */

ROM_START( j6milln )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "pljm-f_1.bin", 0x00000, 0x020000, CRC(a4a9c5b8) SHA1(2fa9f4a7ef05352498b91c2b7bcf4d9ca20614a5) )
	ROM_LOAD16_BYTE( "pljm-f_2.bin", 0x00001, 0x020000, CRC(71f303b2) SHA1(7fba05cb6107296cea9b5be575569daf98f2b9c1) )
	j6milln_sound
ROM_END

ROM_START( j6millna )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "ljc-g1.bin", 0x00000, 0x020000, CRC(518088da) SHA1(d144ab37d17a8ea2174a7ed2dd32b37f3d112dcd) )
	ROM_LOAD16_BYTE( "ljc-g2.bin", 0x00001, 0x020000, CRC(e42c5cc0) SHA1(612973ba3dac2b1098dd746fd4fd6e7bb0246949) )
	j6milln_sound
ROM_END

ROM_START( j6gldpl )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "60000304.p1", 0x00000, 0x020000, CRC(b1a32f63) SHA1(48484bfc7e8dc2937c926ed8c5c4c3f4ebafc908) )
	ROM_LOAD16_BYTE( "60000304.p2", 0x00001, 0x020000, CRC(a7fbc9c7) SHA1(2b2db6a7619011bf02a6b03733387577c0c50cc6) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END

ROM_START( j6shoot )
	ROM_REGION( 0x200000, "maincpu", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "shoot out p1", 0x00000, 0x020000, CRC(d1797023) SHA1(6a75451b0e5954c87f65f333a5fc73d3ac43d1f4) )
	ROM_LOAD16_BYTE( "shootp2", 0x00001, 0x020000, CRC(c345c0a9) SHA1(8e9dfb58b3295b2d44bfa810f5502872cf7111d1) )

	ROM_REGION( 0x80000, "upd", ROMREGION_ERASE00 )
	/* missing? */
ROM_END






/* Mechanical Below */
#define GAME_FLAGS MACHINE_NOT_WORKING|MACHINE_REQUIRES_ARTWORK|MACHINE_MECHANICAL|MACHINE_NO_SOUND|MACHINE_CLICKABLE_ARTWORK

#define GAME_FLAGS_SOUND MACHINE_NOT_WORKING|MACHINE_REQUIRES_ARTWORK|MACHINE_MECHANICAL|MACHINE_CLICKABLE_ARTWORK

#define GAME_FLAGS_WORKING MACHINE_MECHANICAL|MACHINE_CLICKABLE_ARTWORK

// *************************************************************************************************************
// JPM + others games with old 'error log' style codes for errors
// *************************************************************************************************************

GAMEL( 199?, j6aceclb,     0,          impact_nonvideo, j6aceclb, jpmimpct_state, empty_init, ROT0, "Crystal", "Ace Of Clubs (Crystal) (IMPACT, GND D 4)", GAME_FLAGS_WORKING, layout_j6aceclb )
GAMEL( 199?, j6aceclba,    j6aceclb,   impact_nonvideo, j6aceclb, jpmimpct_state, empty_init, ROT0, "Crystal", "Ace Of Clubs (Crystal) (IMPACT, GND D 4 P, Protocol)", GAME_FLAGS_WORKING, layout_j6aceclb )

GAMEL( 199?, j6bnkrcl,     0,          impact_nonvideo, j6bnkrcl, jpmimpct_state, empty_init, ROT0, "JPM", "Banker Club (JPM) (IMPACT) (V6, set 1)", GAME_FLAGS_SOUND, layout_j6bnkrcl )// needs layout rebuild
GAMEL( 199?, j6bnkrcla,    j6bnkrcl,   impact_nonvideo, j6bnkrcl, jpmimpct_state, empty_init, ROT0, "JPM", "Banker Club (JPM) (IMPACT) (V6, set 2)", GAME_FLAGS_SOUND, layout_j6bnkrcl )
GAMEL( 199?, j6bnkrclb,    j6bnkrcl,   impact_nonvideo, j6bnkrcl, jpmimpct_state, empty_init, ROT0, "JPM", "Banker Club (JPM) (IMPACT) (V6, set 3)", GAME_FLAGS_SOUND, layout_j6bnkrcl )
GAMEL( 199?, j6bnkrclc,    j6bnkrcl,   impact_nonvideo, j6bnkrcl, jpmimpct_state, empty_init, ROT0, "JPM", "Banker Club (JPM) (IMPACT) (V2)", GAME_FLAGS_SOUND, layout_j6bnkrcl )

GAMEL( 199?, j6big50,      0,          impact_nonvideo_disc, j6big50, jpmimpct_state, empty_init, ROT0, "JPM",  "Big 50 (JPM) (IMPACT) (BF12 L12)", GAME_FLAGS_WORKING, layout_j6big50 )
GAMEL( 199?, j6big50a,     j6big50,    impact_nonvideo_disc, j6big50, jpmimpct_state, empty_init, ROT0, "JPM",  "Big 50 (JPM) (IMPACT) (BF12P F12, Protocol)", GAME_FLAGS_WORKING, layout_j6big50 )
GAMEL( 199?, j6big50b,     j6big50,    impact_nonvideo_disc, j6big50, jpmimpct_state, empty_init, ROT0, "JPM",  "Big 50 (JPM) (IMPACT) (BF12 AH12)", GAME_FLAGS_WORKING, layout_j6big50 )
GAMEL( 199?, j6big50c,     j6big50,    impact_nonvideo_disc, j6big50, jpmimpct_state, empty_init, ROT0, "JPM",  "Big 50 (JPM) (IMPACT) (BF12 L12, Whitbread)", GAME_FLAGS_WORKING, layout_j6big50 )
GAMEL( 199?, j6big50d,     j6big50,    impact_nonvideo_disc, j6big50, jpmimpct_state, empty_init, ROT0, "JPM",  "Big 50 (JPM) (IMPACT) (BF L8)", GAME_FLAGS_WORKING, layout_j6big50 )

GAMEL( 199?, j6bigbuk,     0,          impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 1)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbuka,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 2)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbukb,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 3)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbukc,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 4)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbukd,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 5)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbuke,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 6)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbukf,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 7)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbukg,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 8)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbukh,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 9)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbuki,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 10)", GAME_FLAGS_WORKING, layout_j6bigbukc )
GAMEL( 199?, j6bigbukj,    j6bigbuk,   impact_nonvideo, j6bigbuk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Bucks (JPM) (IMPACT) (set 11)", GAME_FLAGS, layout_j6bigbukc ) // incomplete pair

// the code in brackets is just what it shows at startup, not sure what it means at the moment
// probably doesn't use the vfd on real hardware, it seems to show debug strings ingame
GAME(  199?, j6bigbnk,     0,          impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB10C  20) (set 1)", GAME_FLAGS )
GAME(  199?, j6bigbnka,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB10C  20) (set 2, Protocol)", GAME_FLAGS )
GAME(  199?, j6bigbnkb,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB10C  20) (set 3)", GAME_FLAGS )
GAME(  199?, j6bigbnkc,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB10C  20) (set 4)", GAME_FLAGS )
GAME(  199?, j6bigbnkd,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB8   H18)", GAME_FLAGS )
GAME(  199?, j6bigbnke,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB8 P H18) (Protocol)", GAME_FLAGS )
GAME(  199?, j6bigbnkf,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB8  AH18)", GAME_FLAGS )
GAME(  199?, j6bigbnkg,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB6 C  16) (set 1, Protocol)", GAME_FLAGS )
GAME(  199?, j6bigbnkh,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB6 C  16) (set 2)", GAME_FLAGS )
GAME(  199?, j6bigbnki,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB2B  H11)", GAME_FLAGS )
GAME(  199?, j6bigbnkj,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB2BP H11) (Protocol)", GAME_FLAGS )
GAME(  199?, j6bigbnkk,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB2B AH11)", GAME_FLAGS )
GAME(  199?, j6bigbnkl,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB2BI H11)", GAME_FLAGS )
GAME(  199?, j6bigbnkm,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB2II H08)", GAME_FLAGS )
GAME(  199?, j6bigbnkn,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB 9C  19)", GAME_FLAGS )
GAME(  199?, j6bigbnko,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB5 I H15)", GAME_FLAGS )
GAME(  199?, j6bigbnkp,    j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB4 I H09)", GAME_FLAGS )
GAME(  199?, j6bbankr,     j6bigbnk,   impact_nonvideo, j6bigbnk, jpmimpct_state, empty_init, ROT0, "JPM", "Big Banker (JPM) (IMPACT) (BB 2  T 2)", GAME_FLAGS ) // was marked as Crystal, but still shows JPM

// missing sound ROM
GAME(  199?, j6bigpct,     0,          impact_nonvideo, j6bigpct, jpmimpct_state, empty_init, ROT0, "Ace", "Big Picture (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6bigpcta,    j6bigpct,   impact_nonvideo, j6bigpct, jpmimpct_state, empty_init, ROT0, "Ace", "Big Picture (Ace) (IMPACT) (set 2, Protocol)", GAME_FLAGS )
GAME(  199?, j6bigpctb,    j6bigpct,   impact_nonvideo, j6bigpct, jpmimpct_state, empty_init, ROT0, "Ace", "Big Picture (Ace) (IMPACT) (set 3)", GAME_FLAGS )

// doesn't coin up (different coin mech? notes?) 4 reels
GAMEL( 199?, j6bigtop,     0,          impact_nonvideo, j6bigtop, jpmimpct_state, empty_init, ROT0, "JPM", "Big Top Club (JPM) (IMPACT) (BT3 BT6) set 1)", GAME_FLAGS, layout_j6bigtopa )
GAMEL( 199?, j6bigtopa,    j6bigtop,   impact_nonvideo, j6bigtop, jpmimpct_state, empty_init, ROT0, "JPM", "Big Top Club (JPM) (IMPACT) (BT4 BT8) (set 2)", GAME_FLAGS, layout_j6bigtopa )
GAMEL( 199?, j6bigtopb,    j6bigtop,   impact_nonvideo, j6bigtop, jpmimpct_state, empty_init, ROT0, "JPM", "Big Top Club (JPM) (IMPACT) (BT4 BT8P) (set 3, Protocol)", GAME_FLAGS, layout_j6bigtopa )
GAMEL( 199?, j6bigtopc,    j6bigtop,   impact_nonvideo, j6bigtop, jpmimpct_state, empty_init, ROT0, "JPM", "Big Top Club (JPM) (IMPACT) (BT4 BT8F) (set 4)", GAME_FLAGS, layout_j6bigtopa )

GAME(  199?, j6bigwhl,     0,          impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Big Wheel (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6bigwhla,    j6bigwhl,   impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Big Wheel (JPM) (IMPACT) (set 2, Protocol)", GAME_FLAGS )
GAME(  199?, j6bigwhlb,    j6bigwhl,   impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Big Wheel (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6bigwhlc,    j6bigwhl,   impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Big Wheel (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6bigwhld,    j6bigwhl,   impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Big Wheel (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6bigwhle,    j6bigwhl,   impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Big Wheel (JPM) (IMPACT) (set 6)", GAME_FLAGS )

// These were marked as Monopoly Bingo but have VFD attract display from Big Wheel, Lamping is different howver, so probably similar to Reel Money case where
// part of the game engine is reused but VFD isn't shown on hardware?
GAME(  199?, j6monobn,     0,          impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly Bingo (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6monobna,    j6bigwhl,   impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly Bingo (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6monobnb,    j6bigwhl,   impact_nonvideo, j6bigwhl, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly Bingo (JPM) (IMPACT) (set 3)", GAME_FLAGS )

GAMEL( 199?, j6bnza,       0,          impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO1   H06)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzaa,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO1 P H06) (Protocol)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzab,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO1  AH06)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzac,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO1 I H06)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzad,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO 9   14)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzae,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO 9P  14) (Protocol)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzaf,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO 9 A 14)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzag,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO 9I  14)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzah,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO5   H10)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzai,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO5 I H10)", GAME_FLAGS_WORKING, layout_j6bnza )
GAMEL( 199?, j6bnzaj,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (incomplete pair)", GAME_FLAGS, layout_j6bnza )
GAMEL( 199?, j6bnzak,      j6bnza,     impact_nonvideo, j6bnza, jpmimpct_state, empty_init, ROT0, "JPM", "Bonanza (JPM) (IMPACT) (BO06   11)", GAME_FLAGS_WORKING, layout_j6bnza )

// 4 reels
GAME(  199?, j6cpclb,      0,          impact_nonvideo, j6cpclb, jpmimpct_state, empty_init, ROT0, "Crystal / JPM", "Caesar's Palace Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cpclba,     j6cpclb,    impact_nonvideo, j6cpclb, jpmimpct_state, empty_init, ROT0, "Crystal / JPM", "Caesar's Palace Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cpclbb,     j6cpclb,    impact_nonvideo, j6cpclb, jpmimpct_state, empty_init, ROT0, "Crystal / JPM", "Caesar's Palace Club (JPM) (IMPACT) (set 3, Protocol)", GAME_FLAGS )

GAMEL( 199?, j6camelt,     0,          impact_nonvideo, j6camelt, jpmimpct_state, empty_init, ROT0, "JPM", "Camelot (JPM) (IMPACT) (CA8 C10)", GAME_FLAGS_WORKING, layout_j6camelt )
GAMEL( 199?, j6camelta,    j6camelt,   impact_nonvideo, j6camelt, jpmimpct_state, empty_init, ROT0, "JPM", "Camelot (JPM) (IMPACT) (CA8P C10, Protocol)", GAME_FLAGS_WORKING, layout_j6camelt )
GAMEL( 199?, j6cameltb,    j6camelt,   impact_nonvideo, j6camelt, jpmimpct_state, empty_init, ROT0, "JPM", "Camelot (JPM) (IMPACT) (CA8 AC10)", GAME_FLAGS_WORKING, layout_j6camelt )
GAMEL( 199?, j6cameltc,    j6camelt,   impact_nonvideo, j6camelt, jpmimpct_state, empty_init, ROT0, "JPM", "Camelot (JPM) (IMPACT) (CA8 C10, Whitbread)", GAME_FLAGS_WORKING, layout_j6camelt )
GAMEL( 199?, j6cameltd,    j6camelt,   impact_nonvideo, j6camelt, jpmimpct_state, empty_init, ROT0, "JPM", "Camelot (JPM) (IMPACT) (CA6 C8)", GAME_FLAGS_WORKING, layout_j6camelt )

GAME(  199?, j6cshbst,     0,          impact_nonvideo, j6cshbst, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Buster (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cshbsta,    j6cshbst,   impact_nonvideo, j6cshbst, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Buster (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cshbstb,    j6cshbst,   impact_nonvideo, j6cshbst, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Buster (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6cshbstc,    j6cshbst,   impact_nonvideo, j6cshbst, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Buster (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6cshbstd,    j6cshbst,   impact_nonvideo, j6cshbst, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Buster (JPM) (IMPACT) (set 5)", GAME_FLAGS )

GAMEL( 199?, j6cascz,      0,          impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC07C L12) (set 1)", GAME_FLAGS_WORKING, layout_j6cascze )
GAMEL( 199?, j6cascza,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC07C L12) (set 2)", GAME_FLAGS_WORKING, layout_j6cascze )
GAMEL( 199?, j6casczb,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6cascze ) // doesn't display anything
GAMEL( 199?, j6casczc,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC11C L16) (set 4)", GAME_FLAGS_WORKING, layout_j6cascze )
GAMEL( 199?, j6casczd,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6cascze ) // doesn't display anything
GAMEL( 199?, j6cascze,     j6cascz,    impact_nonvideo, j6cascze,jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC16C L23) (set 6)", GAME_FLAGS_WORKING, layout_j6cascze )
GAMEL( 199?, j6casczf,     j6cascz,    impact_nonvideo, j6cascze, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC16C L23) (set 7)", GAME_FLAGS_WORKING, layout_j6cascze )
GAMEL( 199?, j6casczg,     j6cascz,    impact_nonvideo, j6cascze, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC16C AL23) (set 8)", GAME_FLAGS, layout_j6cascze )
GAMEL( 199?, j6casczh,     j6cascz,    impact_nonvideo, j6cascze, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC16C L23) (set 9)", GAME_FLAGS_WORKING, layout_j6cascze )
GAMEL( 199?, j6casczi,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC15C L22) (set 10)", GAME_FLAGS, layout_j6cascze ) // ERROR 5.6 but boots
GAMEL( 199?, j6casczj,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (set 11)", GAME_FLAGS, layout_j6cascze ) // doesn't display anything
GAMEL( 199?, j6casczk,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC12C L17) (set 12)", GAME_FLAGS_WORKING, layout_j6cascze )
GAMEL( 199?, j6casczl,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (set 13)", GAME_FLAGS, layout_j6cascze ) // ERROR 91 00 (newer / classic set?)
GAMEL( 199?, j6casczm,     j6cascz,    impact_nonvideo, j6cascz, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy (JPM) (IMPACT) (CC02C L08) (set 14)", GAME_FLAGS_WORKING, layout_j6cascze )

// all of these identify as Casino Crazy and have the same text strings as above? however they do spin 6 reels instead, while the above spin 4
// the VFD is not visible in the artwork for these, so was the game simply built on top of Casino Crazy without removing the VFD messages?
GAMEL( 199?, j6reelmn,     0,           impact_nonvideo, j6reelmn, jpmimpct_state, empty_init, ROT0, "JPM", "Reel Money (JPM) (IMPACT) (RE 9) (set 1)", GAME_FLAGS_WORKING, layout_j6reelmn )
GAMEL( 199?, j6reelmna,    j6reelmn,    impact_nonvideo, j6reelmn, jpmimpct_state, empty_init, ROT0, "JPM", "Reel Money (JPM) (IMPACT) (RE 9) (set 2)", GAME_FLAGS_WORKING, layout_j6reelmn )
GAMEL( 199?, j6reelmnb,    j6reelmn,    impact_nonvideo, j6reelmn, jpmimpct_state, empty_init, ROT0, "JPM", "Reel Money (JPM) (IMPACT) (RE 9) (set 3)", GAME_FLAGS_WORKING, layout_j6reelmn )
GAMEL( 199?, j6reelmnc,    j6reelmn,    impact_nonvideo, j6reelmn, jpmimpct_state, empty_init, ROT0, "JPM", "Reel Money (JPM) (IMPACT) (RE 9) (set 4)", GAME_FLAGS_WORKING, layout_j6reelmn )
GAMEL( 199?, j6reelmnd,    j6reelmn,    impact_nonvideo, j6reelmn, jpmimpct_state, empty_init, ROT0, "JPM", "Reel Money (JPM) (IMPACT) (RE 7) (set 5)", GAME_FLAGS, layout_j6reelmn ) //error 5.5

GAME(  199?, j6casclb,     0,          impact_nonvideo, j6casclb, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6casclba,    j6casclb,   impact_nonvideo, j6casclb, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6ccc,        j6casclb,   impact_nonvideo, j6casclb, jpmimpct_state, empty_init, ROT0, "Crystal", "Casino Crazy Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6ccca,       j6casclb,   impact_nonvideo, j6casclb, jpmimpct_state, empty_init, ROT0, "Crystal", "Casino Crazy Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cccb,       j6casclb,   impact_nonvideo, j6casclb, jpmimpct_state, empty_init, ROT0, "Crystal", "Casino Crazy Club (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6cccc,       j6casclb,   impact_nonvideo, j6casclb, jpmimpct_state, empty_init, ROT0, "Crystal", "Casino Crazy Club (Crystal) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6caslas,     0,          impact_nonvideo, j6caslas, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Las Vegas (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6caslasa,    j6caslas,   impact_nonvideo, j6caslas, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Las Vegas (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6caslasb,    j6caslas,   impact_nonvideo, j6caslas, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Las Vegas (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6caslasc,    j6caslas,   impact_nonvideo, j6caslas, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Las Vegas (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )

GAME(  199?, j6fasttk,     0,          impact_nonvideo, j6fasttk, jpmimpct_state, empty_init, ROT0, "JPM", "Fast?Trak (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6fasttka,    j6fasttk,   impact_nonvideo, j6fasttk, jpmimpct_state, empty_init, ROT0, "JPM", "Fast?Trak (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6fasttkb,    j6fasttk,   impact_nonvideo, j6fasttk, jpmimpct_state, empty_init, ROT0, "JPM", "Fast?Trak (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6fasttkc,    j6fasttk,   impact_nonvideo, j6fasttk, jpmimpct_state, empty_init, ROT0, "JPM", "Fast?Trak (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )

GAME(  199?, j6filth,      0,          impact_nonvideo, j6filth, jpmimpct_state, empty_init, ROT0, "JPM", "Filthy Rich Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6filtha,     j6filth,    impact_nonvideo, j6filth, jpmimpct_state, empty_init, ROT0, "JPM", "Filthy Rich Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6filthb,     j6filth,    impact_nonvideo, j6filth, jpmimpct_state, empty_init, ROT0, "JPM", "Filthy Rich Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6filthc,     j6filth,    impact_nonvideo, j6filth, jpmimpct_state, empty_init, ROT0, "JPM", "Filthy Rich Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6fivalv,     0,          impact_nonvideo, j6fivalv, jpmimpct_state, empty_init, ROT0, "JPM", "Five Alive Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6fivalva,    j6fivalv,   impact_nonvideo, j6fivalv, jpmimpct_state, empty_init, ROT0, "JPM", "Five Alive Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6fivalvb,    j6fivalv,   impact_nonvideo, j6fivalv, jpmimpct_state, empty_init, ROT0, "JPM", "Five Alive Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6frc10,      0,          impact_nonvideo, j6frc10, jpmimpct_state, empty_init, ROT0, "JPM", "Force 10 (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6frc10a,     j6frc10,    impact_nonvideo, j6frc10, jpmimpct_state, empty_init, ROT0, "JPM", "Force 10 (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6frc10b,     j6frc10,    impact_nonvideo, j6frc10, jpmimpct_state, empty_init, ROT0, "JPM", "Force 10 (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6frc10c,     j6frc10,    impact_nonvideo, j6frc10, jpmimpct_state, empty_init, ROT0, "JPM", "Force 10 (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6frc10d,     j6frc10,    impact_nonvideo, j6frc10, jpmimpct_state, empty_init, ROT0, "JPM", "Force 10 (JPM) (IMPACT) (set 5)", GAME_FLAGS )

GAME(  199?, j6fbcrz,      0,          impact_nonvideo, j6fbcrz, jpmimpct_state, empty_init, ROT0, "JPM", "Football Crazy (JPM) (IMPACT)", GAME_FLAGS )

GAMEL( 199?, j6guab,       0,          impact_nonvideo, j6guab, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break (JPM) (IMPACT) (GB 5 C 8) (set 1)", GAME_FLAGS_WORKING, layout_j6guabc )
GAMEL( 199?, j6guaba,      j6guab,     impact_nonvideo, j6guab, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break (JPM) (IMPACT) (GB 5P C 8) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6guabc )
GAMEL( 199?, j6guabb,      j6guab,     impact_nonvideo, j6guab, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break (JPM) (IMPACT) (GB 5 C 8) (set 3, Whitbread)", GAME_FLAGS_WORKING, layout_j6guabc )
GAMEL( 199?, j6guabc,      j6guab,     impact_nonvideo, j6guab, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break (JPM) (IMPACT) (GB 6 C 9) (set 4)", GAME_FLAGS_WORKING, layout_j6guabc )
GAMEL( 199?, j6guabd,      j6guab,     impact_nonvideo, j6guab, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break (JPM) (IMPACT) (GB 6P C 9) (set 5, Protocol)", GAME_FLAGS_WORKING, layout_j6guabc )
GAMEL( 199?, j6guabe,      j6guab,     impact_nonvideo, j6guab, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break (JPM) (IMPACT) (GB 6 C 9) (set 6)", GAME_FLAGS_WORKING, layout_j6guabc )
GAMEL( 199?, j6guabf,      j6guab,     impact_nonvideo, j6guab, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break (JPM) (IMPACT) (GB 6 C 9) (set 7, Whitbread)", GAME_FLAGS_WORKING, layout_j6guabc )

GAMEL( 199?, j6guabcl,     0,          impact_nonvideo, j6guabcl, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break Club (JPM) (IMPACT) (GB 8 GB12) (set 1)", GAME_FLAGS_WORKING, layout_j6guabcl )
GAMEL( 199?, j6guabcla,    j6guabcl,   impact_nonvideo, j6guabcl, jpmimpct_state, empty_init, ROT0, "JPM", "Give Us A Break Club (JPM) (IMPACT) (GB 8 GB12P) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6guabcl )

// was in a set marked Reel Bingo Classic Club, still contains VFD attract strings for Give Us A Break Club, but has different lamping, so likely
// a different game built off the same base code with no VFD showing
GAME( 199?, j6reelb,      0,          impact_nonvideo, j6reelb, jpmimpct_state, empty_init, ROT0, "Crystal", "Reel Bingo Classic Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME( 199?, j6reelba,     j6guabcl,   impact_nonvideo, j6reelb, jpmimpct_state, empty_init, ROT0, "Crystal", "Reel Bingo Classic Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS )

GAMEL( 199?, j6gogold,     0,          impact_nonvideo, j6gogold, jpmimpct_state, empty_init, ROT0, "JPM", "Go For Gold (JPM) (IMPACT) (set 1)", GAME_FLAGS_WORKING, layout_j6gogold )
GAMEL( 199?, j6gogolda,    j6gogold,   impact_nonvideo, j6gogold, jpmimpct_state, empty_init, ROT0, "JPM", "Go For Gold (JPM) (IMPACT) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6gogold )
GAMEL( 199?, j6gogoldb,    j6gogold,   impact_nonvideo, j6gogold, jpmimpct_state, empty_init, ROT0, "JPM", "Go For Gold (JPM) (IMPACT) (set 3)", GAME_FLAGS_WORKING, layout_j6gogold )
GAMEL( 199?, j6gogoldc,    j6gogold,   impact_nonvideo, j6gogold, jpmimpct_state, empty_init, ROT0, "JPM", "Go For Gold (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS_WORKING, layout_j6gogold )

GAMEL( 199?, j6hapyhr,     0,          impact_nonvideo_disc, j6hapyhr, jpmimpct_state, empty_init, ROT0, "JPM", "Happy Hour (JPM) (IMPACT) (HH2 C5) (set 1)", GAME_FLAGS_WORKING, layout_j6hapyhrb )
GAMEL( 199?, j6hapyhra,    j6hapyhr,   impact_nonvideo_disc, j6hapyhr, jpmimpct_state, empty_init, ROT0, "JPM", "Happy Hour (JPM) (IMPACT) (HH2 C5) (set 2)", GAME_FLAGS_WORKING, layout_j6hapyhrb )
GAMEL( 199?, j6hapyhrb,    j6hapyhr,   impact_nonvideo_disc, j6hapyhr, jpmimpct_state, empty_init, ROT0, "JPM", "Happy Hour (JPM) (IMPACT) (HH2 C5) (set 3)", GAME_FLAGS_WORKING, layout_j6hapyhrb )

GAME(  199?, j6hilosv,     0,          impact_nonvideo, j6hilosv, jpmimpct_state, empty_init, ROT0, "JPM", "Hi-Lo Silver (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6hilosva,    j6hilosv,   impact_nonvideo, j6hilosv, jpmimpct_state, empty_init, ROT0, "JPM", "Hi-Lo Silver (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6hilosvb,    j6hilosv,   impact_nonvideo, j6hilosv, jpmimpct_state, empty_init, ROT0, "JPM", "Hi-Lo Silver (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6hilosvc,    j6hilosv,   impact_nonvideo, j6hilosv, jpmimpct_state, empty_init, ROT0, "JPM", "Hi-Lo Silver (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6hilosvd,    j6hilosv,   impact_nonvideo, j6hilosv, jpmimpct_state, empty_init, ROT0, "JPM", "Hi-Lo Silver (JPM) (IMPACT) (set 5)", GAME_FLAGS ) // incomplete pair
GAME(  199?, j6hilosve,    j6hilosv,   impact_nonvideo, j6hilosv, jpmimpct_state, empty_init, ROT0, "JPM", "Hi-Lo Silver (JPM) (IMPACT) (set 6)", GAME_FLAGS ) // incomplete pair

GAMEL( 199?, j6impuls,     0,          impact_nonvideo, j6impuls, jpmimpct_state, empty_init, ROT0, "JPM", "Impulse (JPM) (IMPACT) (IP 5 C13) (set 1)", GAME_FLAGS_WORKING, layout_j6impuls15 )
GAMEL( 199?, j6impulsa,    j6impuls,   impact_nonvideo, j6impulsa,jpmimpct_state, empty_init, ROT0, "JPM", "Impulse (JPM) (IMPACT) (IP 6 C14) (set 2)", GAME_FLAGS_WORKING, layout_j6impuls )
GAMEL( 199?, j6impulsb,    j6impuls,   impact_nonvideo, j6impulsa,jpmimpct_state, empty_init, ROT0, "JPM", "Impulse (JPM) (IMPACT) (IP 6 AC14) (set 3)", GAME_FLAGS_WORKING, layout_j6impuls )
GAMEL( 199?, j6impulsc,    j6impuls,   impact_nonvideo, j6impulsa,jpmimpct_state, empty_init, ROT0, "JPM", "Impulse (JPM) (IMPACT) (IP 7 C15) (set 4)", GAME_FLAGS_WORKING, layout_j6impuls )
GAMEL( 199?, j6impulsd,    j6impuls,   impact_nonvideo, j6impulsa,jpmimpct_state, empty_init, ROT0, "JPM", "Impulse (JPM) (IMPACT) (IP 7 AC13) (set 5)", GAME_FLAGS_WORKING, layout_j6impuls )
GAMEL( 199?, j6impulse,    j6impuls,   impact_nonvideo, j6impulsa,jpmimpct_state, empty_init, ROT0, "JPM", "Impulse (JPM) (IMPACT) (IP 7P C15) (set 6)", GAME_FLAGS_WORKING, layout_j6impuls )
GAMEL( 199?, j6impulsf,    j6impuls,   impact_nonvideo, j6impulsa,jpmimpct_state, empty_init, ROT0, "JPM", "Impulse (JPM) (IMPACT) (IP 7P C15) (set 7, Whitbread)", GAME_FLAGS_WORKING, layout_j6impuls )

// not the same layout or button mapping as the above, but same attract strings?
GAME(  199?, j6impls,      0,          impact_nonvideo, j6impls, jpmimpct_state, empty_init, ROT0, "Crystal", "Impulse (Crystal) (IMPACT)", GAME_FLAGS )

GAMEL( 199?, j6indy,       0,          impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ6 L11) (set 1)", GAME_FLAGS_WORKING, layout_j6indy6 )
GAMEL( 199?, j6indya,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ6P L11) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6indy6 )
GAMEL( 199?, j6indyb,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ6 I L11) (set 3)", GAME_FLAGS_WORKING, layout_j6indy6 )
GAMEL( 199?, j6indyc,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ6A AL13) (set 4)", GAME_FLAGS_WORKING, layout_j6indy6 )
GAMEL( 199?, j6indyd,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ6AP AL13) (set 5, Protocol)", GAME_FLAGS_WORKING, layout_j6indy6 )
GAMEL( 199?, j6indye,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ8 L18) (set 6)", GAME_FLAGS_WORKING, layout_j6indy8)
GAMEL( 199?, j6indyf,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ8P L18) (set 7, Protocol)", GAME_FLAGS_WORKING, layout_j6indy8 )
GAMEL( 199?, j6indyg,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ8 AL18) (set 8)", GAME_FLAGS_WORKING, layout_j6indy8 )
GAMEL( 199?, j6indyh,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ8 I L18) (set 9)", GAME_FLAGS_WORKING, layout_j6indy8 )
GAMEL( 199?, j6indyi,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ11 L22) (set 10)", GAME_FLAGS_WORKING, layout_j6indy8 )
GAMEL( 199?, j6indyj,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ11P L22) (set 11, Protocol)", GAME_FLAGS_WORKING, layout_j6indy8 )
GAMEL( 199?, j6indyk,      j6indy,     impact_nonvideo, j6indy, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT) (IJ11 AL22) (set 12)", GAME_FLAGS_WORKING, layout_j6indy8 )

GAME(  199?, j6jungfv,     0,          impact_nonvideo, j6jungfv, jpmimpct_state, empty_init, ROT0, "Ace", "Jungle Fever (Ace) (IMPACT)", GAME_FLAGS )

GAMEL( 199?, j6kungfu,     0,          impact_nonvideo_altreels, j6kungfu, jpmimpct_state, empty_init, ROT0, "Ace", "Kung Fu (Ace) (IMPACT) (set 1)", GAME_FLAGS_WORKING, layout_j6kungfu )
GAMEL( 199?, j6kungfua,    j6kungfu,   impact_nonvideo_altreels, j6kungfu, jpmimpct_state, empty_init, ROT0, "Ace", "Kung Fu (Ace) (IMPACT) (set 2)", GAME_FLAGS_WORKING, layout_j6kungfu )
GAMEL( 199?, j6kungfub,    j6kungfu,   impact_nonvideo_altreels, j6kungfu, jpmimpct_state, empty_init, ROT0, "Ace", "Kung Fu (Ace) (IMPACT) (set 3)", GAME_FLAGS_WORKING, layout_j6kungfu )
GAMEL( 199?, j6kungfuc,    j6kungfu,   impact_nonvideo_altreels, j6kungfu, jpmimpct_state, empty_init, ROT0, "Ace", "Kung Fu (Ace) (IMPACT) (set 4)", GAME_FLAGS_WORKING, layout_j6kungfu )
GAMEL( 199?, j6kungfud,    j6kungfu,   impact_nonvideo_altreels, j6kungfu, jpmimpct_state, empty_init, ROT0, "Ace", "Kung Fu (Ace) (IMPACT) (set 5, Whitbread)", GAME_FLAGS_WORKING, layout_j6kungfu )

GAME(  199?, j6luckla,     0,          impact_nonvideo, j6luckla, jpmimpct_state, empty_init, ROT0, "Crystal", "Lucky Las Vegas (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6lucklaa,    j6luckla,   impact_nonvideo, j6luckla, jpmimpct_state, empty_init, ROT0, "Crystal", "Lucky Las Vegas (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6lucklab,    j6luckla,   impact_nonvideo, j6luckla, jpmimpct_state, empty_init, ROT0, "Crystal", "Lucky Las Vegas (Crystal) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6mavrk,      0,          impact_nonvideo, j6mavrk, jpmimpct_state, empty_init, ROT0, "JPM", "Maverick (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6mavrka,     j6mavrk,    impact_nonvideo, j6mavrk, jpmimpct_state, empty_init, ROT0, "JPM", "Maverick (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6mavrkb,     j6mavrk,    impact_nonvideo, j6mavrk, jpmimpct_state, empty_init, ROT0, "JPM", "Maverick (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6mavrkc,     j6mavrk,    impact_nonvideo, j6mavrk, jpmimpct_state, empty_init, ROT0, "JPM", "Maverick (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6mavrkd,     j6mavrk,    impact_nonvideo, j6mavrk, jpmimpct_state, empty_init, ROT0, "JPM", "Maverick (JPM) (IMPACT) (set 5)", GAME_FLAGS )

GAME(  199?, j6monmad,     0,          impact_nonvideo, j6monmad, jpmimpct_state, empty_init, ROT0, "Ace", "Money Madness (Ace) (IMPACT)", GAME_FLAGS )

GAMEL( 199?, j6montlk,     0,          impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT5 L17) (set 1)", GAME_FLAGS_WORKING, layout_j6montlk )
GAMEL( 199?, j6montlka,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT5 I L17) (set 2)", GAME_FLAGS_WORKING, layout_j6montlk )
GAMEL( 199?, j6montlkb,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT3 L12) (set 3)", GAME_FLAGS_WORKING, layout_j6montlk6 )
GAMEL( 199?, j6montlkc,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT3 P L12) (set 4, Protocol)", GAME_FLAGS_WORKING, layout_j6montlk6 )
GAMEL( 199?, j6montlkd,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT3 A L12) (set 5)", GAME_FLAGS_WORKING, layout_j6montlk6 )
GAMEL( 199?, j6montlke,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT3 P GL12) (set 6, Protocol)", GAME_FLAGS_WORKING, layout_j6montlk )
GAMEL( 199?, j6montlkf,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT7 L19) (set 7)", GAME_FLAGS_WORKING, layout_j6montlk )
GAMEL( 199?, j6montlkg,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT7 P L19) (set 8, Protocol)", GAME_FLAGS_WORKING, layout_j6montlk )
GAMEL( 199?, j6montlkh,    j6montlk,   impact_nonvideo, j6montlk, jpmimpct_state, empty_init, ROT0, "JPM", "Money Talks (JPM) (IMPACT) (MT7 A L19) (set 9)", GAME_FLAGS_WORKING, layout_j6montlk )

GAMEL( 199?, j6mono60,     0,          impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO13 C24) (set 1)", GAME_FLAGS_WORKING, layout_j6mono60 )
GAMEL( 199?, j6mono60a,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO13 C24) (set 2)", GAME_FLAGS_WORKING, layout_j6mono60 )
GAMEL( 199?, j6mono60b,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO13 C24) (set 3)", GAME_FLAGS_WORKING, layout_j6mono60 )
GAMEL( 199?, j6mono60c,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO13 C24) (set 4, Whitbread)", GAME_FLAGS_WORKING, layout_j6mono60 )
GAMEL( 199?, j6mono60d,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO11 L22) (set 5)", GAME_FLAGS_WORKING, layout_j6mono608 )
GAMEL( 199?, j6mono60e,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO11P L22)(set 6, Protocol)", GAME_FLAGS_WORKING, layout_j6mono608 )
GAMEL( 199?, j6mono60f,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO11 AL22 (set 7)", GAME_FLAGS_WORKING, layout_j6mono608 )
GAMEL( 199?, j6mono60g,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO11 L22) (set 8, Whitbread)", GAME_FLAGS_WORKING, layout_j6mono6010 )
GAMEL( 199?, j6mono60h,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO11 IL22) (set 9)", GAME_FLAGS_WORKING, layout_j6mono608 )
GAMEL( 199?, j6mono60i,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO11 IL22) (set 10)", GAME_FLAGS_WORKING, layout_j6mono608 )
GAMEL( 199?, j6mono60j,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO8 L16) (set 11)", GAME_FLAGS_WORKING, layout_j6mono608 )
GAMEL( 199?, j6mono60k,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (set 12)", GAME_FLAGS, layout_j6mono60 ) // incomplete pair
GAMEL( 199?, j6mono60l,    j6mono60,   impact_nonvideo, j6mono60, jpmimpct_state, empty_init, ROT0, "JPM", "Monopoly 60th Anniversary Edition (JPM) (IMPACT) (MO3 L11) (set 13)", GAME_FLAGS_WORKING, layout_j6mono608 )

GAME(  199?, j6outlaw,     0,          impact_nonvideo, j6outlaw, jpmimpct_state, empty_init, ROT0, "JPM", "Outlaw (JPM) (IMPACT, v3)", GAME_FLAGS )
GAME(  199?, j6outlawd,    j6outlaw,   impact_nonvideo, j6outlaw, jpmimpct_state, empty_init, ROT0, "JPM", "Outlaw (JPM) (IMPACT, v3) (Protocol)", GAME_FLAGS )
GAME(  199?, j6outlawc,    j6outlaw,   impact_nonvideo, j6outlaw, jpmimpct_state, empty_init, ROT0, "JPM", "Outlaw (JPM) (IMPACT, Club?)", GAME_FLAGS )

GAME(  199?, j6oxo,        0,          impact_nonvideo, j6oxo, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6oxoa,       j6oxo,      impact_nonvideo, j6oxo, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6oxob,       j6oxo,      impact_nonvideo, j6oxo, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6oxoc,       j6oxo,      impact_nonvideo, j6oxo, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6oxod,       j6oxo,      impact_nonvideo, j6oxo, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6oxoe,       j6oxo,      impact_nonvideo, j6oxo, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo (JPM) (IMPACT) (set 6)", GAME_FLAGS )

GAMEL( 199?, j6oxobin,     0,          impact_nonvideo, j6oxobin, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo Bingo (JPM) (IMPACT) (OX6 L12) (set 1)", GAME_FLAGS_WORKING, layout_j6oxobin )
GAMEL( 199?, j6oxobina,    j6oxobin,   impact_nonvideo, j6oxobin, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo Bingo (JPM) (IMPACT) (OX6 L12) (set 2)", GAME_FLAGS_WORKING, layout_j6oxobin )
GAMEL( 199?, j6oxobinb,    j6oxobin,   impact_nonvideo, j6oxobin, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo Bingo (JPM) (IMPACT) (OX8 AC14) (set 3)", GAME_FLAGS, layout_j6oxobin10 ) //Different layout lamping,like j6oxo?
GAMEL( 199?, j6oxobinc,    j6oxobin,   impact_nonvideo, j6oxobin, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo Bingo (JPM) (IMPACT) (OX8 AC14) (set 4)", GAME_FLAGS, layout_j6oxobin10 )
GAMEL( 199?, j6oxobind,    j6oxobin,   impact_nonvideo, j6oxobin, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo Bingo (JPM) (IMPACT) (OX11 AC17) (set 5)", GAME_FLAGS, layout_j6oxobin10 )
GAMEL( 199?, j6oxobine,    j6oxobin,   impact_nonvideo, j6oxobin, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo Bingo (JPM) (IMPACT) (set 6)", GAME_FLAGS, layout_j6oxobin ) // ERROR 4.4
GAMEL( 199?, j6oxobinf,    j6oxobin,   impact_nonvideo, j6oxobin, jpmimpct_state, empty_init, ROT0, "JPM", "Oxo Bingo (JPM) (IMPACT) (OX11 AC17) (set 7)", GAME_FLAGS, layout_j6oxobin10 )

GAME(  199?, j6popoli,     0,          impact_nonvideo, j6popoli, jpmimpct_state, empty_init, ROT0, "JPM", "Popeye & Olive (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6popolia,    j6popoli,   impact_nonvideo, j6popoli, jpmimpct_state, empty_init, ROT0, "JPM", "Popeye & Olive (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6pog,        0,          impact_nonvideo_altreels, j6pog, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold (Ace) (IMPACT) (set 1)", GAME_FLAGS ) // has video roms, why? (misplaced? hybrid?)
GAME(  199?, j6poga,       j6pog,      impact_nonvideo_altreels, j6pog, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6pogb,       j6pog,      impact_nonvideo_altreels, j6pog, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6pogc,       j6pog,      impact_nonvideo_altreels, j6pog, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold (Ace) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6pogd,       j6pog,      impact_nonvideo_altreels, j6pog, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold (Ace) (IMPACT) (set 5)", GAME_FLAGS )
// same as above but with Crystal text
GAME(  199?, j6potg,       j6pog,      impact_nonvideo_altreels, j6potg, jpmimpct_state, empty_init, ROT0, "Crystal", "Pot Of Gold (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6potga,      j6pog,      impact_nonvideo_altreels, j6potg, jpmimpct_state, empty_init, ROT0, "Crystal", "Pot Of Gold (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6potgb,      j6pog,      impact_nonvideo_altreels, j6potg, jpmimpct_state, empty_init, ROT0, "Crystal", "Pot Of Gold (Crystal) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6pwrspn,     0,          impact_nonvideo, j6pwrspn, jpmimpct_state, empty_init, ROT0, "JPM", "Powerspin (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pwrspna,    j6pwrspn,   impact_nonvideo, j6pwrspn, jpmimpct_state, empty_init, ROT0, "JPM", "Powerspin (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6pwrspnb,    j6pwrspn,   impact_nonvideo, j6pwrspn, jpmimpct_state, empty_init, ROT0, "JPM", "Powerspin (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6pwrspnc,    j6pwrspn,   impact_nonvideo, j6pwrspn, jpmimpct_state, empty_init, ROT0, "JPM", "Powerspin (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6pwrspnd,    j6pwrspn,   impact_nonvideo, j6pwrspn, jpmimpct_state, empty_init, ROT0, "JPM", "Powerspin (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6pwrspne,    j6pwrspn,   impact_nonvideo, j6pwrspn, jpmimpct_state, empty_init, ROT0, "JPM", "Powerspin (JPM) (IMPACT) (set 6)", GAME_FLAGS ) // ERROR 8.1

GAMEL( 199?, j6quick,      0,          impact_nonvideo, j6quick, jpmimpct_state, empty_init, ROT0, "RAL", "Quicksilver (RAL) (HLB AC2) (IMPACT)", GAME_FLAGS_WORKING, layout_j6quick )

GAME(  199?, j6richpk,     0,          impact_nonvideo, j6richpk, jpmimpct_state, empty_init, ROT0, "Ace", "Rich Pickings (Ace) (IMPACT)", GAME_FLAGS )

GAMEL( 199?, j6rico,       0,          impact_nonvideo, j6rico, jpmimpct_state, empty_init, ROT0, "JPM", "Ricochet (JPM) (IMPACT) (R I) (set 1)", GAME_FLAGS_WORKING, layout_j6rico10 )
GAMEL( 199?, j6ricoa,      j6rico,     impact_nonvideo, j6rico, jpmimpct_state, empty_init, ROT0, "JPM", "Ricochet (JPM) (IMPACT) (R IP) (set 2)", GAME_FLAGS_WORKING, layout_j6rico10 )
GAMEL( 199?, j6ricob,      j6rico,     impact_nonvideo, j6rico, jpmimpct_state, empty_init, ROT0, "JPM", "Ricochet (JPM) (IMPACT) (R IA) (set 3)", GAME_FLAGS_WORKING, layout_j6rico10 )
GAMEL( 199?, j6ricoc,      j6rico,     impact_nonvideo, j6rico, jpmimpct_state, empty_init, ROT0, "JPM", "Ricochet (JPM) (IMPACT) (R I) (set 4)", GAME_FLAGS_WORKING, layout_j6rico10 )
GAMEL( 199?, j6ricod,      j6rico,     impact_nonvideo, j6rico, jpmimpct_state, empty_init, ROT0, "JPM", "Ricochet (JPM) (IMPACT) (R IS) (set 5)", GAME_FLAGS_WORKING, layout_j6rico10 )
GAMEL( 199?, j6ricoe,      j6rico,     impact_nonvideo, j6rico, jpmimpct_state, empty_init, ROT0, "JPM", "Ricochet (JPM) (IMPACT) (R IS) (set 6)", GAME_FLAGS_WORKING, layout_j6rico10 )

GAME(  199?, j6robin,      0,          impact_nonvideo_altreels, j6robin, jpmimpct_state, empty_init, ROT0, "Ace", "Robin Hood (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6robina,     j6robin,    impact_nonvideo_altreels, j6robin, jpmimpct_state, empty_init, ROT0, "Ace", "Robin Hood (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6robinb,     j6robin,    impact_nonvideo_altreels, j6robin, jpmimpct_state, empty_init, ROT0, "Ace", "Robin Hood (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6robinc,     j6robin,    impact_nonvideo_altreels, j6robin, jpmimpct_state, empty_init, ROT0, "Ace", "Robin Hood (Ace) (IMPACT) (set 4)", GAME_FLAGS )


GAMEL( 199?, j6roller,     0,          impact_nonvideo, j6roller15, jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC11 C30, 30p Stake, £15 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller15 )
GAMEL( 199?, j6rollera,    j6roller,   impact_nonvideo, j6roller15, jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC11P C30, 30p Stake, £15 Jackpot, Protocol)", GAME_FLAGS_WORKING, layout_j6roller15 )
GAMEL( 199?, j6rollerb,    j6roller,   impact_nonvideo, j6roller15, jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC11 AC30, 30p Stake, £15 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller15 )
GAMEL( 199?, j6rollerc,    j6roller,   impact_nonvideo, j6roller15, jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC11 C30, 30p Stake, £15 Jackpot, Whitbread)", GAME_FLAGS_WORKING, layout_j6roller15 )
GAMEL( 199?, j6rollerd,    j6roller,   impact_nonvideo, j6roller15, jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC11 C29, 30p Stake, £15 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller15 )
GAMEL( 199?, j6rollere,    j6roller,   impact_nonvideo, j6roller10, jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC10 C20, 20p Stake, £10 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller10 )
GAMEL( 199?, j6rollerf,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC9 L26, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollerg,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC9P L26, 20p Stake, £8 Jackpot, Protocol)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollerh,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC9 AL26, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rolleri,    j6roller,   impact_nonvideo, j6roller10, jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC9 L26, 20p Stake, £10 Jackpot, Whitbread)", GAME_FLAGS_WORKING, layout_j6roller10 )
GAMEL( 199?, j6rollerj,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC5 L14, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollerk,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC4 L15, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollerl,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC4P L15, 20p Stake, £8 Jackpot, Protocol)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollerm,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC4 AL15, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollern,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC4 I L15, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollero,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC2 L07, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )
GAMEL( 199?, j6rollerp,    j6roller,   impact_nonvideo, j6roller8,  jpmimpct_state, empty_init, ROT0, "JPM", u8"Roller Coaster (JPM) (IMPACT) (RC2 I L07, 20p Stake, £8 Jackpot)", GAME_FLAGS_WORKING, layout_j6roller8 )

GAME(  199?, j6rcclub,     0,          impact_nonvideo, j6rcclub, jpmimpct_state, empty_init, ROT0, "JPM", "Roller Coaster Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6rccluba,    j6rcclub,   impact_nonvideo, j6rcclub, jpmimpct_state, empty_init, ROT0, "JPM", "Roller Coaster Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6rcclubb,    j6rcclub,   impact_nonvideo, j6rcclub, jpmimpct_state, empty_init, ROT0, "JPM", "Roller Coaster Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6royfls,     0,          impact_nonvideo, j6royfls, jpmimpct_state, empty_init, ROT0, "JPM", "Royal Flush Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6royflsa,    j6royfls,   impact_nonvideo, j6royfls, jpmimpct_state, empty_init, ROT0, "JPM", "Royal Flush Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6royflsb,    j6royfls,   impact_nonvideo, j6royfls, jpmimpct_state, empty_init, ROT0, "JPM", "Royal Flush Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6royflsc,    j6royfls,   impact_nonvideo, j6royfls, jpmimpct_state, empty_init, ROT0, "JPM", "Royal Flush Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6royflsd,    j6royfls,   impact_nonvideo, j6royfls, jpmimpct_state, empty_init, ROT0, "JPM", "Royal Flush Club (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6royflse,    j6royfls,   impact_nonvideo, j6royfls, jpmimpct_state, empty_init, ROT0, "JPM", "Royal Flush Club (JPM) (IMPACT) (set 6)", GAME_FLAGS )

GAME(  199?, j6sidewd,     0,          impact_nonvideo, j6sidewd, jpmimpct_state, empty_init, ROT0, "JPM", "Sidewinder (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6sidewda,    j6sidewd,   impact_nonvideo, j6sidewd, jpmimpct_state, empty_init, ROT0, "JPM", "Sidewinder (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6sidewdb,    j6sidewd,   impact_nonvideo, j6sidewd, jpmimpct_state, empty_init, ROT0, "JPM", "Sidewinder (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6sidewdc,    j6sidewd,   impact_nonvideo, j6sidewd, jpmimpct_state, empty_init, ROT0, "JPM", "Sidewinder (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6sidewdd,    j6sidewd,   impact_nonvideo, j6sidewd, jpmimpct_state, empty_init, ROT0, "JPM", "Sidewinder (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6sidewde,    j6sidewd,   impact_nonvideo, j6sidewd, jpmimpct_state, empty_init, ROT0, "JPM", "Sidewinder (JPM) (IMPACT) (set 6)", GAME_FLAGS )
//
GAMEL( 199?, j6snakes,     0,          impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL18 C22) (set 1)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesa,    j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL18P C22) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesb,    j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL18 AC22) (set 3)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesc,    j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL18 C22) (set 4, Whitbread)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesd,    j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL16 C20) (set 5)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakese,    j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL17 C21) (set 6)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesf,    j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL10 C13) (set 7)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesg,    j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (IMPACT) (SL8P C11) (set 8)", GAME_FLAGS_WORKING, layout_j6snakes )
// these were in 'slides again' sets, but appear to be the original game
GAMEL( 199?, j6snakesh,     j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (SL11 C14) (IMPACT) (set 9)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesi,     j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (SL11 AC14) (IMPACT) (set 10)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesj,     j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (SL11P CA4) (IMPACT) (set 11, Protocol)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesk,     j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM", "Snakes & Ladders (JPM) (SL15 C18) (IMPACT) (set 12)", GAME_FLAGS_WORKING, layout_j6snakes )
GAMEL( 199?, j6snakesl,     j6snakes,   impact_nonvideo, j6snakes, jpmimpct_state, empty_init, ROT0, "JPM / Crystal", "Snakes & Ladders (JPM) (SLB C2) (IMPACT) (set 13)", GAME_FLAGS, layout_j6snakes ) // 5.5 Invalid Key?

GAMEL( 199?, j6stards,     0,          impact_nonvideo, j6stards, jpmimpct_state, empty_init, ROT0, "JPM", "Stardust (JPM) (IMPACT) (SD 2 C 7) (set 1)", GAME_FLAGS_WORKING, layout_j6stards )
GAMEL( 199?, j6stardsa,    j6stards,   impact_nonvideo, j6stards, jpmimpct_state, empty_init, ROT0, "JPM", "Stardust (JPM) (IMPACT) (SD 2P C 7) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6stards )
GAMEL( 199?, j6stardsb,    j6stards,   impact_nonvideo, j6stards, jpmimpct_state, empty_init, ROT0, "JPM", "Stardust (JPM) (IMPACT) (SD 2 AC 7) (set 3)", GAME_FLAGS_WORKING, layout_j6stards )
GAMEL( 199?, j6stardsc,    j6stards,   impact_nonvideo, j6stards, jpmimpct_state, empty_init, ROT0, "JPM", "Stardust (JPM) (IMPACT) (SD 2 C 7) (set 4, Whitbread)", GAME_FLAGS_WORKING, layout_j6stards )

GAME(  199?, j6strk10,     0,          impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6strk10a,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6strk10b,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6strk10c,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6strk10d,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6strk10e,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6strk10f,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6strk10g,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6strk10h,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6strk10i,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6strk10j,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6strk10k,    j6strk10,   impact_nonvideo, j6strk10, jpmimpct_state, empty_init, ROT0, "Ace", "Strike 10 (Ace) (IMPACT) (set 12)", GAME_FLAGS )

GAMEL(  199?, j6sonic,      0,          impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO10 C13)", GAME_FLAGS_WORKING, layout_j6sonic10 )
GAMEL(  199?, j6sonica,     j6sonic,    impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO10P C13, Protocol)", GAME_FLAGS, layout_j6sonic10 ) // Error 7.1 "Electronic Data Counter Timeout"
GAMEL(  199?, j6sonicb,     j6sonic,    impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO10 AC13)", GAME_FLAGS_WORKING, layout_j6sonic10  )
GAMEL(  199?, j6sonicc,     j6sonic,    impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO10 C13, Whitbread)", GAME_FLAGS_WORKING, layout_j6sonic10  )
GAMEL(  199?, j6sonicd,     j6sonic,    impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO9 C12)", GAME_FLAGS_WORKING, layout_j6sonic10  )
GAMEL(  199?, j6sonice,     j6sonic,    impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO7 C10)", GAME_FLAGS_WORKING, layout_j6sonic10  )
GAMEL(  199?, j6sonicf,     j6sonic,    impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO7 AC10)", GAME_FLAGS_WORKING, layout_j6sonic10  )
GAMEL(  199?, j6sonicg,     j6sonic,    impact_nonvideo, j6sonicg,jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (SO1 AC2)", GAME_FLAGS_WORKING, layout_j6sonic10  )
GAME(  199?, j6sonich,     j6sonic,    impact_nonvideo, j6sonic, jpmimpct_state, empty_init, ROT0, "JPM", "Sonic The Hedgehog (JPM) (IMPACT) (set 9)", GAME_FLAGS ) // incomplete pair

// needs to be reset once to boot
GAME(  199?, j6roof,       0,          impact_nonvideo, j6roof, jpmimpct_state, empty_init, ROT0, "Ace", "Thru' The Roof (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6roofa,      j6roof,     impact_nonvideo, j6roof, jpmimpct_state, empty_init, ROT0, "Ace", "Thru' The Roof (Ace) (IMPACT) (set 2)", GAME_FLAGS )

GAMEL( 199?, j6untch,      0,          impact_nonvideo, j6untch, jpmimpct_state, empty_init, ROT0, "JPM", "Untouchable (JPM) (IMPACT) (UN3B LO6)", GAME_FLAGS_WORKING, layout_j6untch )
GAMEL( 199?, j6untcha,     j6untch,    impact_nonvideo, j6untch, jpmimpct_state, empty_init, ROT0, "JPM", "Untouchable (JPM) (IMPACT) (UN3BI LO6)", GAME_FLAGS_WORKING, layout_j6untch )

// VFD issues, but VFD is likely unused as it calls the game 'Some Indeterminate Club Machine' with clear placeholder text
GAMEL( 199?, j6wldkng,     0,          impact_nonvideo, j6wldkng, jpmimpct_state, empty_init, ROT0, "JPM", "Wild King Club (JPM) (IMPACT) (WK 5) (set 1)", GAME_FLAGS_WORKING, layout_j6wldkng )
GAMEL( 199?, j6wldknga,    j6wldkng,   impact_nonvideo, j6wldkng, jpmimpct_state, empty_init, ROT0, "JPM", "Wild King Club (JPM) (IMPACT) (WK 5P) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6wldkng )

GAME(  199?, j6wildw,      0,          impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6wildwa,     j6wildw,    impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6wildwb,     j6wildw,    impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 3, Whitbread)", GAME_FLAGS )
GAME(  199?, j6wildwc,     j6wildw,    impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6wildwd,     j6wildw,    impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6wildwe,     j6wildw,    impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6wildwf,     j6wildw,    impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6wildwg,     j6wildw,    impact_nonvideo, j6wildw, jpmimpct_state, empty_init, ROT0, "Ace", "Wild West (Ace) (IMPACT) (set 8, Whitbread)", GAME_FLAGS )

GAMEL( 199?, j6wizard,     0,          impact_nonvideo, j6wizard, jpmimpct_state, empty_init, ROT0, "JPM", "Wizard Of Odds (JPM) (IMPACT) (WZ 7 C 8) (set 1)", GAME_FLAGS_WORKING, layout_j6wizardd )
GAMEL( 199?, j6wizarda,    j6wizard,   impact_nonvideo, j6wizard, jpmimpct_state, empty_init, ROT0, "JPM", "Wizard Of Odds (JPM) (IMPACT) (WZ 7P C 8) (set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6wizardd )
GAMEL( 199?, j6wizardb,    j6wizard,   impact_nonvideo, j6wizard, jpmimpct_state, empty_init, ROT0, "JPM", "Wizard Of Odds (JPM) (IMPACT) (WZ 7 AC 8) (set 3)", GAME_FLAGS_WORKING, layout_j6wizardd )
GAMEL( 199?, j6wizardc,    j6wizard,   impact_nonvideo, j6wizard, jpmimpct_state, empty_init, ROT0, "JPM", "Wizard Of Odds (JPM) (IMPACT) (WZ 7 C 8) (set 4, Whitbread)", GAME_FLAGS_WORKING, layout_j6wizardd )
GAMEL( 199?, j6wizardd,    j6wizard,   impact_nonvideo, j6wizard, jpmimpct_state, empty_init, ROT0, "JPM", "Wizard Of Odds (JPM) (IMPACT) (WZ 4 C 5) (set 5)", GAME_FLAGS_WORKING, layout_j6wizardd )
GAMEL( 199?, j6wizarde,    j6wizard,   impact_nonvideo, j6wizard, jpmimpct_state, empty_init, ROT0, "JPM", "Wizard Of Odds (JPM) (IMPACT) (WZ 5 C 6) (set 6)", GAME_FLAGS_WORKING, layout_j6wizardd )

GAME(  199?, j6hikar,      0,          impact_nonvideo, j6hikar, jpmimpct_state, empty_init, ROT0, "Crystal", "Hi Karate (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6hikara,     j6hikar,    impact_nonvideo, j6hikar, jpmimpct_state, empty_init, ROT0, "Crystal", "Hi Karate (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6hikarb,     j6hikar,    impact_nonvideo, j6hikar, jpmimpct_state, empty_init, ROT0, "Crystal", "Hi Karate (Crystal) (IMPACT) (set 3)", GAME_FLAGS )

// vfd issues (probably not shown?)
GAME(  199?, j6slvgst,     0,          impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6slvgsta,    j6slvgst,   impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6slvgstb,    j6slvgst,   impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6slvgstc,    j6slvgst,   impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 4)", GAME_FLAGS ) // this set gives error 4.4 (% key missing) even if one is present in usual place?
GAME(  199?, j6slvgstd,    j6slvgst,   impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6slvgste,    j6slvgst,   impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6slvgstf,    j6slvgst,   impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6slvgstg,    j6slvgst,   impact_nonvideo, j6slvgst, jpmimpct_state, empty_init, ROT0, "JPM", "Silver Ghost (JPM) (IMPACT) (set 8)", GAME_FLAGS )

GAME(  199?, j6h5clb,      0,          impact_nonvideo, j6h5clb, jpmimpct_state, empty_init, ROT0, "JPM", "High Five Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6h5clba,     j6h5clb,    impact_nonvideo, j6h5clb, jpmimpct_state, empty_init, ROT0, "JPM", "High Five Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )

// needs to be reset one to boot
GAME(  199?, j6cdivr,      0,          impact_nonvideo, j6cdivr, jpmimpct_state, empty_init, ROT0, "Crystal", "Cash Diver (Crystal) (IMPACT)", GAME_FLAGS )

// needs to be reset one to boot
GAME(  199?, j6grc,        0,          impact_nonvideo, j6grc, jpmimpct_state, empty_init, ROT0, "Crystal", "Gold Rush Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6grca,       j6grc,      impact_nonvideo, j6grc, jpmimpct_state, empty_init, ROT0, "Crystal", "Gold Rush Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6thril,      0,          impact_nonvideo, j6thril, jpmimpct_state, empty_init, ROT0, "Crystal", "Thriller (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6thrila,     j6thril,    impact_nonvideo, j6thril, jpmimpct_state, empty_init, ROT0, "Crystal", "Thriller (Crystal) (IMPACT) (set 2)", GAME_FLAGS )

// *************************************************************************************************************
// QPS games with old 'error log' style codes for errors
// *************************************************************************************************************

// these still give 5.7, which is the hopper / pay verification error, so either need a better hookup, or an entirely different one

GAME(  199?, j6coprob,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Cops 'n' Robbers (Qps) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6coproba,    j6coprob,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Cops 'n' Robbers (Qps) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6coprobb,    j6coprob,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Cops 'n' Robbers (Qps) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6coprobc,    j6coprob,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Cops 'n' Robbers (Qps) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6coprobd,    j6coprob,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Cops 'n' Robbers (Qps) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6coprobe,    j6coprob,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Cops 'n' Robbers (Qps) (IMPACT) (set 6)", GAME_FLAGS )

GAMEL( 199?, j6drdogh,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Dr Dough (Qps) (IMPACT)", GAME_FLAGS, layout_j6drdogh )

GAMEL( 199?, j6fastfr,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Fast Fruits Club (Qps) (IMPACT) (set 1)", GAME_FLAGS, layout_j6fastfr )
GAMEL( 199?, j6fastfra,    j6fastfr,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Fast Fruits Club (Qps) (IMPACT) (set 2)", GAME_FLAGS, layout_j6fastfr )

GAMEL( 199?, j6frtpot,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Fruitpots (Qps) (IMPACT) (set 1)", GAME_FLAGS, layout_j6frtpot )
GAMEL( 199?, j6frtpota,    j6frtpot,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Fruitpots (Qps) (IMPACT) (set 2)", GAME_FLAGS, layout_j6frtpot )
GAMEL( 199?, j6frtpotb,    j6frtpot,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Fruitpots (Qps) (IMPACT) (set 3)", GAME_FLAGS, layout_j6frtpot )
GAMEL( 199?, j6frtpotc,    j6frtpot,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Fruitpots (Qps) (IMPACT) (set 4)", GAME_FLAGS, layout_j6frtpot )

GAME(  199?, j6histk,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Hi Stakes (Qps) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6histka,     j6histk,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Hi Stakes (Qps) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6jackjs,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Jackpot Justice (Qps) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6jackjsa,    j6jackjs,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Jackpot Justice (Qps) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6jackjsb,    j6jackjs,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Jackpot Justice (Qps) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6jackjsc,    j6jackjs,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Jackpot Justice (Qps) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6gldpl,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Golden Palace (Qps) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6medal,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Medallion Job (Qps) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6medala,     j6medal,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Medallion Job (Qps) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6medalb,     j6medal,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Medallion Job (Qps) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6medalc,     j6medal,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Medallion Job (Qps) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6medald,     j6medal,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Medallion Job (Qps) (IMPACT) (set 5)", GAME_FLAGS )

GAME(  199?, j6pacman,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Pac Man Plus (Qps) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6tutti,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Qps", "Tutti Frutti (Qps) (IMPACT)", GAME_FLAGS )

// *************************************************************************************************************
// JPM + others games with 'Bacta 6 Standard Codes' for errors
// these include some of the 'Classic' reissues
// *************************************************************************************************************

GAME(  199?, j6acehi,      0,          impact_nonvideo, j6acehi, jpmimpct_state, empty_init, ROT0, "Ace", "Aces High (Ace) (IMPACT)", GAME_FLAGS )

GAMEL( 199?, j6arcade,     0,          impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR9 C10, V9, set 1)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadea,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR9 A C10, V9, set 2)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadeb,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR9 C10, V9, set 3)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadec,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR9 P, V9, set 4, Protocol)", GAME_FLAGS, layout_j6arcadee ) // gives error 71 00 even with datapak attached?
GAMEL( 199?, j6arcaded,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR9 P C10, V9, set 5, Protocol)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadee,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR10 C11, V10, set 1, Protocol)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadef,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR10 A C11, V10, set 2, Protocol)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadeg,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR10 C11, V10, set 3, Protocol)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadeh,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR10 P, V10, set 4, Protocol)", GAME_FLAGS, layout_j6arcadee ) // gives error 71 00 even with datapak attached?
GAMEL( 199?, j6arcadei,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR10 P C11, V10, set 5, Protocol)", GAME_FLAGS_WORKING, layout_j6arcadee )
GAMEL( 199?, j6arcadej,    j6arcade,   impact_nonvideo, j6arcade, jpmimpct_state, empty_init, ROT0, "JPM", "Arcadia (JPM) (IMPACT) (AR10 C11, V10, set 6, Protocol?)", GAME_FLAGS_WORKING, layout_j6arcadee )

GAMEL( 199?, j6colcsh,     0,          impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6colcsh ) //inputs
GAMEL( 199?, j6colcsha,    j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6colcsh )
GAMEL( 199?, j6colcshb,    j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6colcsh )
GAMEL( 199?, j6colcshc,    j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6colcsh )
GAMEL( 199?, j6colcshd,    j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6colcsh ) // gives error 71 00 even with datapak attached?
GAMEL( 199?, j6colb,       j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 6)", GAME_FLAGS, layout_j6colcsh )
GAMEL( 199?, j6colc,       j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 7)", GAME_FLAGS, layout_j6colcsh )
GAMEL( 199?, j6cold,       j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 8)", GAME_FLAGS, layout_j6colcsh )
GAMEL( 199?, j6cole,       j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 9)", GAME_FLAGS, layout_j6colcsh )
GAMEL( 199?, j6colf,       j6colcsh,   impact_nonvideo, j6colcsh, jpmimpct_state, empty_init, ROT0, "JPM", "Coliseum Cash (JPM) (IMPACT) (set 10)", GAME_FLAGS, layout_j6colcsh )

GAME(  199?, j6bucks,      0,          impact_nonvideo, j6bucks, jpmimpct_state, empty_init, ROT0, "Ace", "Bucks Fizz (Ace) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6scarlt,     0,          impact_nonvideo, j6scarlt, jpmimpct_state, empty_init, ROT0, "Ace", "Captain Scarlet (Ace) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6cshrd,      0,          impact_nonvideo, j6cshrd, jpmimpct_state, empty_init, ROT0, "Ace", "Cash Raider (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cshrda,     j6cshrd,    impact_nonvideo, j6cshrd, jpmimpct_state, empty_init, ROT0, "Ace", "Cash Raider (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cshrdb,     j6cshrd,    impact_nonvideo, j6cshrd, jpmimpct_state, empty_init, ROT0, "Ace", "Cash Raider (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6cshrdc,     j6cshrd,    impact_nonvideo, j6cshrd, jpmimpct_state, empty_init, ROT0, "Ace", "Cash Raider (Ace) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6cshrdd,     j6cshrd,    impact_nonvideo, j6cshrd, jpmimpct_state, empty_init, ROT0, "Ace", "Cash Raider (Ace) (IMPACT) (set 5)", GAME_FLAGS )

GAME(  199?, j6cshtwr,     0,          impact_nonvideo, j6cshtwr, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Towers (JPM) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6cshvgs,     0,          impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cshvgsa,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cshvgsb,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6cshvgsc,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6cshvgsd,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 5)", GAME_FLAGS ) // gives error 71 00 even with datapak attached?
GAME(  199?, j6cshvgse,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6cshvgsf,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6cshvgsg,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6cshvgsh,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6cshvgsi,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6cshvgsj,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6cshvgsk,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 12)", GAME_FLAGS )
GAME(  199?, j6cshvgsl,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 13)", GAME_FLAGS ) // gives error 71 00 even with datapak attached?
GAME(  199?, j6cshvgsm,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 14)", GAME_FLAGS )
GAME(  199?, j6cshvgsn,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 15)", GAME_FLAGS )
GAME(  199?, j6cshvgso,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 16)", GAME_FLAGS )
GAME(  199?, j6cshvgsp,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 17)", GAME_FLAGS )
GAME(  199?, j6cshvgsq,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 18)", GAME_FLAGS )
GAME(  199?, j6cshvgsr,    j6cshvgs,   impact_nonvideo, j6cshvgs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Vegas Strip (JPM) (IMPACT) (set 19)", GAME_FLAGS )

GAME(  199?, j6cas5,       0,          impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cas5a,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cas5b,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6cas5c,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6cas5d,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6cas5e,      j6cas5,     impact_nonvideo, j6cas5e,jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6cas5f,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6cas5g,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 8, Whitbread)", GAME_FLAGS )
GAME(  199?, j6cas5h,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6cas5i,      j6cas5,     impact_nonvideo, j6cas5e,jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6cas5j,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6cas5k,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 12, Whitbread)", GAME_FLAGS )
GAME(  199?, j6cas5l,      j6cas5,     impact_nonvideo, j6cas5, jpmimpct_state, empty_init, ROT0, "JPM", "Casino 5ive Liner (JPM) (IMPACT) (set 13)", GAME_FLAGS )

GAME(  199?, j6cccla,      0,          impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6ccclaa,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6ccclab,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6ccclac,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6ccclad,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6ccclae,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6ccclaf,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6ccclag,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 8)", GAME_FLAGS ) // ERROR 91 01
GAME(  199?, j6ccclah,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6ccclai,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6ccclaj,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6ccclak,     j6cccla,    impact_nonvideo, j6cccla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic (JPM) (IMPACT) (set 12)", GAME_FLAGS )

// is this the same as the above? both identify as 'Club'
GAMEL( 199?, j6cascla,     0,          impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclaa,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclab,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclac,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclad,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6casclaf ) // ERROR 91 01 Control Pointer Omitted
GAMEL( 199?, j6casclae,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 6)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclaf,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 7)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclag,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 8)", GAME_FLAGS, layout_j6casclaf ) // ERROR 91 01
GAMEL( 199?, j6casclah,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 9)", GAME_FLAGS, layout_j6casclaf ) // ERROR 91 01
GAMEL( 199?, j6casclai,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 10)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclaj,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 11)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclak,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 12)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclal,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 13)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclam,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 14)", GAME_FLAGS, layout_j6casclaf ) // ERROR 91 01
GAMEL( 199?, j6casclan,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 15)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclao,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 16)", GAME_FLAGS, layout_j6casclaf ) // ERROR 16 00 Note Acceptor Fault
GAMEL( 199?, j6casclap,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 17)", GAME_FLAGS, layout_j6casclaf ) // ERROR 16 00
GAMEL( 199?, j6casclaq,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 18)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclar,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 19)", GAME_FLAGS, layout_j6casclaf ) // ERROR 91 01
GAMEL( 199?, j6casclas,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 20)", GAME_FLAGS, layout_j6casclaf )
GAMEL( 199?, j6casclat,    j6cascla,   impact_nonvideo, j6cascla, jpmimpct_state, empty_init, ROT0, "JPM", "Casino Crazy Classic Club (JPM) (IMPACT) (set 21)", GAME_FLAGS, layout_j6casclaf )

GAME(  199?, j6cheque,     0,          impact_nonvideo, j6cheque, jpmimpct_state, empty_init, ROT0, "JPM / Predator", "Cheque Mate (JPM) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6cluclb,     0,          impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cluclba,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cluclbb,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6cluclbc,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6cluclbd,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 5)", GAME_FLAGS ) // ERROR 16 00
GAME(  199?, j6cluclbe,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6cluclbf,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 7)", GAME_FLAGS ) // ERROR 16 00
GAME(  199?, j6cluclbg,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6cluclbh,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 9)", GAME_FLAGS ) // ERROR 16 00
GAME(  199?, j6cluclbi,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6cluclbj,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6cluclbk,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 12)", GAME_FLAGS )
GAME(  199?, j6cluclbl,    j6cluclb,   impact_nonvideo, j6cluclb, jpmimpct_state, empty_init, ROT0, "JPM", "Cluedo Club (JPM) (IMPACT) (set 13)", GAME_FLAGS )

GAME(  199?, j6colmon,     0,          impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 1, Whitbread)", GAME_FLAGS )
GAME(  199?, j6colmona,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6colmonb,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6colmonc,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6colmond,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6colmone,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 6, Whitbread)", GAME_FLAGS )
GAME(  199?, j6colmonf,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6colmong,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6colmonh,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6colmoni,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6colmonj,    j6colmon,   impact_nonvideo, j6colmon, jpmimpct_state, empty_init, ROT0, "JPM", "Colour Of Money (JPM) (IMPACT) (set 11)", GAME_FLAGS ) // ERROR 71 00 (dataport) first boot, ok after reset?


GAMEL( 199?, j6crack,      0,          impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6cracka,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6crackb,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6crackc,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6crack ) // ERROR 71 00 (datapak)
GAMEL( 199?, j6crackd,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6cracke,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 6, Whitbread)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6crackf,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 7)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6crackg,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 8)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6crackh,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 9)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6cracki,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 10)", GAME_FLAGS, layout_j6crack )
GAMEL( 199?, j6crackj,     j6crack,    impact_nonvideo, j6crack, jpmimpct_state, empty_init, ROT0, "JPM", "Cracker (JPM) (IMPACT) (set 11)", GAME_FLAGS, layout_j6crack )
GAME(  199?, j6crakr,      j6crack,    impact_nonvideo, j6crakr, jpmimpct_state, empty_init, ROT0, "Crystal", "Cracker (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6crakra,     j6crack,    impact_nonvideo, j6crakr, jpmimpct_state, empty_init, ROT0, "Crystal", "Cracker (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6crakrb,     j6crack,    impact_nonvideo, j6crakr, jpmimpct_state, empty_init, ROT0, "Crystal", "Cracker (Crystal) (IMPACT) (set 3)", GAME_FLAGS )



GAME(  199?, j6crzclb,     0,          impact_nonvideo, j6crzclb, jpmimpct_state, empty_init, ROT0, "JPM", "Crazy Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6crzclba,    j6crzclb,   impact_nonvideo, j6crzclb, jpmimpct_state, empty_init, ROT0, "JPM", "Crazy Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6crzclbb,    j6crzclb,   impact_nonvideo, j6crzclb, jpmimpct_state, empty_init, ROT0, "JPM", "Crazy Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6crzclbc,    j6crzclb,   impact_nonvideo, j6crzclb, jpmimpct_state, empty_init, ROT0, "JPM", "Crazy Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6dmngz,      0,          impact_nonvideo, j6dmngz, jpmimpct_state, empty_init, ROT0, "JPM", "Diamond Geezer (JPM) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6dyfl,       0,          impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6dyfla,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6dyflb,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6dyflc,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 4)", GAME_FLAGS ) // ERROR 71 00
GAME(  199?, j6dyfld,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6dyfle,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 6, Whitbread)", GAME_FLAGS )
GAME(  199?, j6dyflf,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6dyflg,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6dyflh,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6dyfli,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6dyflj,      j6dyfl,     impact_nonvideo, j6dyfl, jpmimpct_state, empty_init, ROT0, "JPM", "Do You Feel Lucky (JPM) (IMPACT) (set 11)", GAME_FLAGS ) // ERROR 71 00

GAMEL( 199?, j6euphor,     0,          impact_nonvideo, j6euphor, jpmimpct_state, empty_init, ROT0, "Ace", "Euphoria (Ace) (IMPACT) (set 1)", GAME_FLAGS, layout_j6euphor )
GAMEL( 199?, j6euphora,    j6euphor,   impact_nonvideo, j6euphor, jpmimpct_state, empty_init, ROT0, "Ace", "Euphoria (Ace) (IMPACT) (set 2)", GAME_FLAGS, layout_j6euphor )
GAMEL( 199?, j6euphorb,    j6euphor,   impact_nonvideo, j6euphor, jpmimpct_state, empty_init, ROT0, "Ace", "Euphoria (Ace) (IMPACT) (set 3)", GAME_FLAGS, layout_j6euphor )
GAMEL( 199?, j6euphorc,    j6euphor,   impact_nonvideo, j6euphor, jpmimpct_state, empty_init, ROT0, "Ace", "Euphoria (Ace) (IMPACT) (set 4)", GAME_FLAGS, layout_j6euphor )
GAMEL( 199?, j6euphord,    j6euphor,   impact_nonvideo, j6euphor, jpmimpct_state, empty_init, ROT0, "Ace", "Euphoria (Ace) (IMPACT) (set 5)", GAME_FLAGS, layout_j6euphor )
GAMEL( 199?, j6euphore,    j6euphor,   impact_nonvideo, j6euphor, jpmimpct_state, empty_init, ROT0, "Ace", "Euphoria (Ace) (IMPACT) (set 6)", GAME_FLAGS, layout_j6euphor ) // ERROR 71 00
GAMEL( 199?, j6euphorf,    j6euphor,   impact_nonvideo, j6euphorf, jpmimpct_state, empty_init, ROT0, "Ace", "Euphoria (Ace) (IMPACT) (set 7)", GAME_FLAGS, layout_j6euphor )

GAME(  199?, j6firbl,      0,          impact_nonvideo, j6firbl, jpmimpct_state, empty_init, ROT0, "Ace", "Fireball (Ace) (IMPACT) (set 1, Whitbread)", GAME_FLAGS )
GAME(  199?, j6firbla,     j6firbl,    impact_nonvideo, j6firbl, jpmimpct_state, empty_init, ROT0, "Ace", "Fireball (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6firblb,     j6firbl,    impact_nonvideo, j6firblb,jpmimpct_state, empty_init, ROT0, "Ace", "Fireball (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6firblc,     j6firbl,    impact_nonvideo, j6firbl, jpmimpct_state, empty_init, ROT0, "Ace", "Fireball (Ace) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6firbld,     j6firbl,    impact_nonvideo, j6firbl, jpmimpct_state, empty_init, ROT0, "Ace", "Fireball (Ace) (IMPACT) (set 5, Whitbread)", GAME_FLAGS )

GAME(  199?, j6fireck,     0,          impact_nonvideo, j6fireck, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6firecka,    j6fireck,   impact_nonvideo, j6fireck, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6fireckb,    j6fireck,   impact_nonvideo, j6fireck, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6fireckc,    j6fireck,   impact_nonvideo, j6fireck, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6fireckd,    j6fireck,   impact_nonvideo, j6fireck, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6firecke,    j6fireck,   impact_nonvideo, j6fireck, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker (JPM) (IMPACT) (set 6)", GAME_FLAGS )

GAME(  199?, j6firclb,     0,          impact_nonvideo, j6firclb, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6firclba,    j6firclb,   impact_nonvideo, j6firclb, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6firclbb,    j6firclb,   impact_nonvideo, j6firclb, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6firclbc,    j6firclb,   impact_nonvideo, j6firclb, jpmimpct_state, empty_init, ROT0, "JPM", "Firecracker Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6fiveln,     0,          impact_nonvideo, j6fiveln, jpmimpct_state, empty_init, ROT0, "JPM", "Five Liner (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6fivelna,    j6fiveln,   impact_nonvideo, j6fiveln, jpmimpct_state, empty_init, ROT0, "JPM", "Five Liner (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6fivelnb,    j6fiveln,   impact_nonvideo, j6fiveln, jpmimpct_state, empty_init, ROT0, "JPM", "Five Liner (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6fivelnc,    j6fiveln,   impact_nonvideo, j6fiveln, jpmimpct_state, empty_init, ROT0, "JPM", "Five Liner (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )

GAME(  199?, j6framft,     0,          impact_nonvideo, j6framft, jpmimpct_state, empty_init, ROT0, "JPM", "Frame & Fortune Club (JPM) (IMPACT)", GAME_FLAGS )
GAMEL( 199?, j6ffc,        j6framft,   impact_nonvideo, j6framft, jpmimpct_state, empty_init, ROT0, "Crystal", "Frame & Fortune Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6ffce )
GAMEL( 199?, j6ffca,       j6framft,   impact_nonvideo, j6framft, jpmimpct_state, empty_init, ROT0, "Crystal", "Frame & Fortune Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6ffce )
GAMEL( 199?, j6ffcb,       j6framft,   impact_nonvideo, j6framft, jpmimpct_state, empty_init, ROT0, "Crystal", "Frame & Fortune Club (Crystal) (IMPACT) (set 3)", GAME_FLAGS, layout_j6ffce )
GAMEL( 199?, j6ffcc,       j6framft,   impact_nonvideo, j6framft, jpmimpct_state, empty_init, ROT0, "Crystal", "Frame & Fortune Club (Crystal) (IMPACT) (set 4)", GAME_FLAGS, layout_j6ffce )
GAMEL( 199?, j6ffcd,       j6framft,   impact_nonvideo, j6framft, jpmimpct_state, empty_init, ROT0, "Crystal", "Frame & Fortune Club (Crystal) (IMPACT) (set 5)", GAME_FLAGS, layout_j6ffce )
GAMEL( 199?, j6ffce,       j6framft,   impact_nonvideo, j6framft, jpmimpct_state, empty_init, ROT0, "Crystal", "Frame & Fortune Club (Crystal) (IMPACT) (set 6)", GAME_FLAGS, layout_j6ffce )

GAME(  199?, j6frtmch,     0,          impact_nonvideo, j6frtmch, jpmimpct_state, empty_init, ROT0, "JPM", "The Fruit Machine (JPM) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6gforce,     0,          impact_nonvideo, j6gforce, jpmimpct_state, empty_init, ROT0, "JPM", "G Force Roller Coaster (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6gforcea,    j6gforce,   impact_nonvideo, j6gforcea,jpmimpct_state, empty_init, ROT0, "JPM", "G Force Roller Coaster (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6gforceb,    j6gforce,   impact_nonvideo, j6gforce, jpmimpct_state, empty_init, ROT0, "JPM", "G Force Roller Coaster (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6gforcec,    j6gforce,   impact_nonvideo, j6gforce, jpmimpct_state, empty_init, ROT0, "JPM", "G Force Roller Coaster (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6gforced,    j6gforce,   impact_nonvideo, j6gforce, jpmimpct_state, empty_init, ROT0, "JPM", "G Force Roller Coaster (JPM) (IMPACT) (set 5)", GAME_FLAGS )

GAMEL( 199?, j6gidogh,     0,          impact_nonvideo, j6gidogh, jpmimpct_state, empty_init, ROT0, "Ace", "G.I. Dough (Ace) (IMPACT)", GAME_FLAGS, layout_j6gidogh )

GAMEL( 199?, j6gldclb,     0,          impact_nonvideo, j6gldclb, jpmimpct_state, empty_init, ROT0, "JPM", "Gladiator Club (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6gldclb )
GAMEL( 199?, j6gldclba,    j6gldclb,   impact_nonvideo, j6gldclb, jpmimpct_state, empty_init, ROT0, "JPM", "Gladiator Club (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6gldclb )
GAMEL( 199?, j6gldclbb,    j6gldclb,   impact_nonvideo, j6gldclb, jpmimpct_state, empty_init, ROT0, "JPM", "Gladiator Club (JPM) (IMPACT) (set 3, Protocol)", GAME_FLAGS, layout_j6gldclb )

GAMEL( 199?, j6goldgl,     0,          impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6goldgla ) // IOU 5.00
GAMEL( 199?, j6goldgla,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldglb,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldglc,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldgld,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldgle,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 6)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldglf,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 7)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldglg,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 8)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldglh,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 9)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldgli,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 10)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldglj,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 11)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldglk,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 12)", GAME_FLAGS, layout_j6goldgla )
GAMEL( 199?, j6goldgll,    j6goldgl,   impact_nonvideo, j6goldgl, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Goal (JPM) (IMPACT) (set 13)", GAME_FLAGS, layout_j6goldgla )

GAME(  199?, j6hifly,      0,          impact_nonvideo, j6hifly, jpmimpct_state, empty_init, ROT0, "Ace", "Hi Flyer (Ace) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6impact,     0,          impact_nonvideo, j6impact, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Impact (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6impacta,    j6impact,   impact_nonvideo, j6impact, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Impact (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6impactb,    j6impact,   impact_nonvideo, j6impact, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Impact (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6impactc,    j6impact,   impact_nonvideo, j6impact, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Impact (JPM) (IMPACT) (15GBP Jackpot)", GAME_FLAGS )

GAME(  199?, j6hirol,      0,          impact_nonvideo, j6hirol, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6hirola,     j6hirol,    impact_nonvideo, j6hirol, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6hirolb,     j6hirol,    impact_nonvideo, j6hirol, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6hirolc,     j6hirol,    impact_nonvideo, j6hirol, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6hirold,     j6hirol,    impact_nonvideo, j6hirol, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller (JPM) (IMPACT) (set 5, Whitbread)", GAME_FLAGS )

GAME(  199?, j6hirlcl,     0,          impact_nonvideo, j6hirlcl, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6hirlcla,    j6hirlcl,   impact_nonvideo, j6hirlcl, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6hirlclb,    j6hirlcl,   impact_nonvideo, j6hirlcl, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6hirlclc,    j6hirlcl,   impact_nonvideo, j6hirlcl, jpmimpct_state, empty_init, ROT0, "JPM", "Hi Roller Club (JPM) (IMPACT) (set 4, Protocol)", GAME_FLAGS )

GAME(  199?, j6hiphop,     0,          impact_nonvideo, j6hiphop, jpmimpct_state, empty_init, ROT0, "Ace", "Hip Hopper (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6hiphopa,    j6hiphop,   impact_nonvideo, j6hiphop, jpmimpct_state, empty_init, ROT0, "Ace", "Hip Hopper (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6hiphopb,    j6hiphop,   impact_nonvideo, j6hiphop, jpmimpct_state, empty_init, ROT0, "Ace", "Hip Hopper (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6hiphopc,    j6hiphop,   impact_nonvideo, j6hiphop, jpmimpct_state, empty_init, ROT0, "Ace", "Hip Hopper (Ace) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6hiphopd,    j6hiphop,   impact_nonvideo, j6hiphop, jpmimpct_state, empty_init, ROT0, "Ace", "Hip Hopper (Ace) (IMPACT) (set 5)", GAME_FLAGS ) // ERROR 71 00

GAME(  199?, j6hotsht,     0,          impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6hotshta,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6hotshtb,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6hotshtc,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6hotshtd,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6hotshte,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6hotshtf,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6hotshtg,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6hotshth,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6hotshti,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 10)", GAME_FLAGS ) // ERROR 5.1 RAM RESET (?)
GAME(  199?, j6hotshtj,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 11)", GAME_FLAGS ) // ERROR 5.1
GAME(  199?, j6hotshtk,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 12)", GAME_FLAGS ) // ERROR 5.1
GAME(  199?, j6hotshtl,    j6hotsht,   impact_nonvideo, j6hotsht, jpmimpct_state, empty_init, ROT0, "Ace", "Hot Shot (Ace) (IMPACT) (set 13)", GAME_FLAGS ) // ERROR 5.1

GAME(  199?, j6showtm,     0,          impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6showtma,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6showtmb,    j6showtm,   impact_nonvideo, j6showtmb,jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6showtmc,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6showtmd,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6showtme,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6showtmf,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6showtmg,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 8)", GAME_FLAGS ) // ERROR 71 00
GAME(  199?, j6showtmh,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6showtmi,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6showtmj,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6showtmk,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 12)", GAME_FLAGS )
GAME(  199?, j6showtml,    j6showtm,   impact_nonvideo, j6showtm, jpmimpct_state, empty_init, ROT0, "JPM", "It's Showtime (JPM) (IMPACT) (set 13)", GAME_FLAGS )

GAME(  199?, j6magcir,     0,          impact_nonvideo, j6magcir, jpmimpct_state, empty_init, ROT0, "JPM", "Magic Circle Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6magcira,    j6magcir,   impact_nonvideo, j6magcir, jpmimpct_state, empty_init, ROT0, "JPM", "Magic Circle Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6magcirb,    j6magcir,   impact_nonvideo, j6magcir, jpmimpct_state, empty_init, ROT0, "JPM", "Magic Circle Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6magcirc,    j6magcir,   impact_nonvideo, j6magcir, jpmimpct_state, empty_init, ROT0, "JPM", "Magic Circle Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6magcird,    j6magcir,   impact_nonvideo, j6magcir, jpmimpct_state, empty_init, ROT0, "JPM", "Magic Circle Club (JPM) (IMPACT) (set 5)", GAME_FLAGS )

GAME(  199?, j6maxod,      0,          impact_nonvideo, j6maxod, jpmimpct_state, empty_init, ROT0, "JPM", "Maximum Overdrive (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6maxoda,     j6maxod,    impact_nonvideo, j6maxod, jpmimpct_state, empty_init, ROT0, "JPM", "Maximum Overdrive (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6maxodb,     j6maxod,    impact_nonvideo, j6maxod, jpmimpct_state, empty_init, ROT0, "JPM", "Maximum Overdrive (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6maxodc,     j6maxod,    impact_nonvideo, j6maxod, jpmimpct_state, empty_init, ROT0, "JPM", "Maximum Overdrive (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )

GAMEL( 199?, j6megbck,     0,          impact_nonvideo, j6megbck, jpmimpct_state, empty_init, ROT0, "JPM", "Mega Bucks (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6megbck )
GAMEL( 199?, j6megbcka,    j6megbck,   impact_nonvideo, j6megbck, jpmimpct_state, empty_init, ROT0, "JPM", "Mega Bucks (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6megbck )
GAMEL( 199?, j6megbckb,    j6megbck,   impact_nonvideo, j6megbck, jpmimpct_state, empty_init, ROT0, "JPM", "Mega Bucks (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6megbck )
GAMEL( 199?, j6megbckc,    j6megbck,   impact_nonvideo, j6megbck, jpmimpct_state, empty_init, ROT0, "JPM", "Mega Bucks (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6megbck )
GAMEL( 199?, j6megbckd,    j6megbck,   impact_nonvideo, j6megbck, jpmimpct_state, empty_init, ROT0, "JPM", "Mega Bucks (JPM) (IMPACT) (set 5, Whitbread)", GAME_FLAGS, layout_j6megbck )

GAME(  199?, j6pinwzd,     0,          impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "JPM", "Pinball Wizard (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pinwzda,    j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "JPM", "Pinball Wizard (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6pinwzdb,    j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "JPM", "Pinball Wizard (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6pinwzdc,    j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "JPM", "Pinball Wizard (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )
GAME(  199?, j6pinwzdd,    j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "JPM", "Pinball Wizard (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6pinwzde,    j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "JPM", "Pinball Wizard (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6pinwzc,     j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "Crystal", "Pinball Wizard (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pinwzca,    j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "Crystal", "Pinball Wizard (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6pinwzcb,    j6pinwzd,   impact_nonvideo, j6pinwzd, jpmimpct_state, empty_init, ROT0, "Crystal", "Pinball Wizard (Crystal) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6pogcls,     0,          impact_nonvideo, j6pogcls, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold Classic (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pogclsa,    j6pogcls,   impact_nonvideo, j6pogcls, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold Classic (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6pogclsb,    j6pogcls,   impact_nonvideo, j6pogcls, jpmimpct_state, empty_init, ROT0, "Ace", "Pot Of Gold Classic (Ace) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6quantm,     0,          impact_nonvideo, j6quantm, jpmimpct_state, empty_init, ROT0, "JPM", "Quantum Leap (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6quantma,    j6quantm,   impact_nonvideo, j6quantm, jpmimpct_state, empty_init, ROT0, "JPM", "Quantum Leap (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6quantmb,    j6quantm,   impact_nonvideo, j6quantm, jpmimpct_state, empty_init, ROT0, "JPM", "Quantum Leap (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6quantmc,    j6quantm,   impact_nonvideo, j6quantm, jpmimpct_state, empty_init, ROT0, "JPM", "Quantum Leap (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )

GAMEL( 199?, j6ra,         0,          impact_nonvideo, j6ra, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6ra )
GAMEL( 199?, j6raa,        j6ra,       impact_nonvideo, j6ra, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6ra )
GAMEL( 199?, j6rab,        j6ra,       impact_nonvideo, j6ra, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6ra )
GAMEL( 199?, j6rac,        j6ra,       impact_nonvideo, j6ra, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert (JPM) (IMPACT) (set 4, Whitbread)", GAME_FLAGS, layout_j6ra )
GAMEL( 199?, j6rad,        j6ra,       impact_nonvideo, j6ra, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6ra )
GAMEL( 199?, j6redal,      j6ra,       impact_nonvideo, j6ra, jpmimpct_state, empty_init, ROT0, "Crystal", "Red Alert (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6ra )
GAMEL( 199?, j6redala,     j6ra,       impact_nonvideo, j6ra, jpmimpct_state, empty_init, ROT0, "Crystal", "Red Alert (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6ra )

GAME(  199?, j6raclb,      0,          impact_nonvideo, j6raclb, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6raclba,     j6raclb,    impact_nonvideo, j6raclb, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6raclbb,     j6raclb,    impact_nonvideo, j6raclb, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6raclbc,     j6raclb,    impact_nonvideo, j6raclb, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6raclbd,     j6raclb,    impact_nonvideo, j6raclb, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert Club (JPM) (IMPACT) (set 5)", GAME_FLAGS )

GAMEL( 199?, j6redarw,     0,          impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwa,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwb,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwc,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6redarwf ) // ERROR 71 00
GAMEL( 199?, j6redarwd,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6redarwf ) // ERROR 71 00
GAMEL( 199?, j6redarwe,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 6, Whitbread)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwf,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 7)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwg,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 8)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwh,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 9)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwi,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 10)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarwj,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 11)", GAME_FLAGS, layout_j6redarwf )
GAMEL( 199?, j6redarww,    j6redarw,   impact_nonvideo, j6redarw, jpmimpct_state, empty_init, ROT0, "JPM", "Red Arrow (JPM) (IMPACT) (set 12, Whitbread)", GAME_FLAGS, layout_j6redarwf )

GAME( 2002, j6rh6,        0,          impact_nonvideo, j6rh6, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot 6 (JPM) (IMPACT) (set 1)", GAME_FLAGS ) // ERROR 91 01
GAME( 2002, j6rh6a,       j6rh6,      impact_nonvideo, j6rh6, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot 6 (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME( 2002, j6rh6b,       j6rh6,      impact_nonvideo, j6rh6, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot 6 (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME( 2002, j6rh6c,       j6rh6,      impact_nonvideo, j6rh6, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot 6 (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME( 2002, j6rh6d,       j6rh6,      impact_nonvideo, j6rh6, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot 6 (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME( 2002, j6rh6e,       j6rh6,      impact_nonvideo, j6rh6, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot 6 (JPM) (IMPACT) (set 6)", GAME_FLAGS ) // ERROR 71 00

GAMEL( 199?, j6rhchil,     0,          impact_nonvideo, j6rhchil, jpmimpct_state, empty_init, ROT0, "Ace", "Red Hot Chili Stepper (Ace) (IMPACT) (set 1)", GAME_FLAGS, layout_j6rhchil )
GAMEL( 199?, j6rhchila,    j6rhchil,   impact_nonvideo, j6rhchil, jpmimpct_state, empty_init, ROT0, "Ace", "Red Hot Chili Stepper (Ace) (IMPACT) (set 2)", GAME_FLAGS, layout_j6rhchil )
GAMEL( 199?, j6rhchilb,    j6rhchil,   impact_nonvideo, j6rhchil, jpmimpct_state, empty_init, ROT0, "Ace", "Red Hot Chili Stepper (Ace) (IMPACT) (set 3)", GAME_FLAGS, layout_j6rhchil )
GAMEL( 199?, j6rhchilc,    j6rhchil,   impact_nonvideo, j6rhchil, jpmimpct_state, empty_init, ROT0, "Ace", "Red Hot Chili Stepper (Ace) (IMPACT) (set 4)", GAME_FLAGS, layout_j6rhchil )
GAMEL( 199?, j6rhchild,    j6rhchil,   impact_nonvideo, j6rhchil, jpmimpct_state, empty_init, ROT0, "Ace", "Red Hot Chili Stepper (Ace) (IMPACT) (set 5)", GAME_FLAGS, layout_j6rhchil )

GAMEL( 199?, j6rh6cl,      0,          impact_nonvideo, j6rh6cl, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot Six Club (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6rh6cld )
GAMEL( 199?, j6rh6cla,     j6rh6cl,    impact_nonvideo, j6rh6cl, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot Six Club (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6rh6cld )
GAMEL( 199?, j6rh6clb,     j6rh6cl,    impact_nonvideo, j6rh6cl, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot Six Club (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6rh6cld )
GAMEL( 199?, j6rh6clc,     j6rh6cl,    impact_nonvideo, j6rh6cl, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot Six Club (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6rh6cld ) // ERROR 71 00
GAMEL( 199?, j6rh6cld,     j6rh6cl,    impact_nonvideo, j6rh6cl, jpmimpct_state, empty_init, ROT0, "JPM", "Red Hot Six Club (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6rh6cld )

GAME(  199?, j6reelth,     0,          impact_nonvideo, j6reelth, jpmimpct_state, empty_init, ROT0, "Ace", "Reel Thing (Ace) (IMPACT) set 1)", GAME_FLAGS )
GAME(  199?, j6reeltha,    j6reelth,   impact_nonvideo, j6reelth, jpmimpct_state, empty_init, ROT0, "Crystal", "Reel Thing (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6reelthb,    j6reelth,   impact_nonvideo, j6reelth, jpmimpct_state, empty_init, ROT0, "Crystal", "Reel Thing (Crystal) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6rccls,      0,          impact_nonvideo, j6rccls, jpmimpct_state, empty_init, ROT0, "JPM", "Roller Coaster Classic (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6rcclsa,     j6rccls,    impact_nonvideo, j6rccls, jpmimpct_state, empty_init, ROT0, "JPM", "Roller Coaster Classic (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6rcclsb,     j6rccls,    impact_nonvideo, j6rccls, jpmimpct_state, empty_init, ROT0, "JPM", "Roller Coaster Classic (JPM) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6samur,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Samurai Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6samura,     j6samur,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Samurai Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6samurb,     j6samur,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Samurai Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6samurc,     j6samur,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Samurai Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6samurd,     j6samur,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Samurai Club (JPM) (IMPACT) (set 5)", GAME_FLAGS )

GAME(  199?, j6spcinv,     0,          impact_nonvideo, j6spcinv, jpmimpct_state, empty_init, ROT0, "Crystal", "Space Invaders (Crystal) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6supbrk,     0,          impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6supbrka,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6supbrkb,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6supbrkc,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 4)", GAME_FLAGS ) // ERROR 71 00
GAME(  199?, j6supbrkd,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 5)", GAME_FLAGS ) // ERROR 71 00
GAME(  199?, j6supbrke,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 6, Whitbread)", GAME_FLAGS )
GAME(  199?, j6supbrkf,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6supbrkg,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6supbrkh,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6supbrki,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6supbrkj,    j6supbrk,   impact_nonvideo, j6supbrk, jpmimpct_state, empty_init, ROT0, "JPM", "Super Breakout (JPM) (IMPACT) (set 11)", GAME_FLAGS ) // ERROR 71 00

GAMEL( 199?, j6swpdrp,     0,          impact_nonvideo, j6swpdrp, jpmimpct_state, empty_init, ROT0, "JPM", "Swop Till Ya Drop (JPM) (IMPACT)", GAME_FLAGS, layout_j6swpdrp )

GAME(  199?, j6bags,       0,          impact_nonvideo, j6bags, jpmimpct_state, empty_init, ROT0, "JPM", "Three Bags Full (JPM) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6tbirds,     0,          impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6tbirdsa,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6tbirdsb,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6tbirdsc,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6tbirdsd,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 5)", GAME_FLAGS ) // ERROR 71 00
GAME(  199?, j6tbirdse,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 6, Whitbread)", GAME_FLAGS )
GAME(  199?, j6tbirdsf,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6tbirdsg,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6tbirdsh,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 9)", GAME_FLAGS ) // ERROR 71 00
GAME(  199?, j6tbirdsi,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6tbirdsj,    j6tbirds,   impact_nonvideo, j6tbirds, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds (JPM) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6tbirdsk,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "Crystal", "Thunderbirds (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6tbirdsl,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "Crystal", "Thunderbirds (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6tbirdsm,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "Crystal", "Thunderbirds (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6tbirdsn,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "Crystal", "Thunderbirds (Crystal) (IMPACT) (set 4)", GAME_FLAGS ) // bad dump or mismatched pair
GAME(  199?, j6tbirdso,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "Crystal", "Thunderbirds (Crystal) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6tbirdsp,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "Crystal", "Thunderbirds (Crystal) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6tbirdsq,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "Crystal", "Thunderbirds (Crystal) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6tbirdsr,    j6tbirds,   impact_nonvideo, j6tbirdsk, jpmimpct_state, empty_init, ROT0, "JPM / Predator", "Thunderbirds (JPM / Predator) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6tbrdcl,     0,          impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6tbrdcla,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6tbrdclb,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6tbrdclc,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6tbrdcld,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 5)", GAME_FLAGS ) // ERROR 71 00
GAME(  199?, j6tbrdcle,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6tbrdclf,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6tbrdclg,    j6tbrdcl,   impact_nonvideo, j6tbrdclg,jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 8)", GAME_FLAGS )
GAME(  199?, j6tbrdclh,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 9)", GAME_FLAGS )
GAME(  199?, j6tbrdcli,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 10)", GAME_FLAGS )
GAME(  199?, j6tbrdclj,    j6tbrdcl,   impact_nonvideo, j6tbrdclg,jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 11)", GAME_FLAGS )
GAME(  199?, j6tbrdclk,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 12)", GAME_FLAGS )
GAME(  199?, j6tbrdcll,    j6tbrdcl,   impact_nonvideo, j6tbrdcl, jpmimpct_state, empty_init, ROT0, "JPM", "Thunderbirds Club (JPM) (IMPACT) (set 13)", GAME_FLAGS )

GAMEL( 199?, j6tomb,       0,          impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6tombc ) //inputs
GAMEL( 199?, j6tomba,      j6tomb,     impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6tombc )
GAMEL( 199?, j6tombb,      j6tomb,     impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6tombc )
GAMEL( 199?, j6tombc,      j6tomb,     impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6tombc )
GAMEL( 199?, j6tombd,      j6tomb,     impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 5)", GAME_FLAGS, layout_j6tombc )
GAMEL( 199?, j6tombe,      j6tomb,     impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 6)", GAME_FLAGS, layout_j6tombc )
GAMEL( 199?, j6tombf,      j6tomb,     impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 7)", GAME_FLAGS, layout_j6tombc ) // ERROR 71 00
GAMEL( 199?, j6tombg,      j6tomb,     impact_nonvideo, j6tomb, jpmimpct_state, empty_init, ROT0, "JPM", "Tomb Raider (JPM) (IMPACT) (set 8)", GAME_FLAGS, layout_j6tombc )

GAMEL( 199?, j6topflg,     0,          impact_nonvideo, j6topflg, jpmimpct_state, empty_init, ROT0, "Ace", "Top Flight (Ace) (IMPACT)", GAME_FLAGS, layout_j6topflg )

GAMEL( 199?, j6twst,       0,          impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 1)", GAME_FLAGS, layout_j6twst ) //inputs
GAMEL( 199?, j6twsta,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 2)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twstb,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 3)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twstc,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 4)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twstd,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 5)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twste,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 6)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twstf,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 7)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twstg,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 8)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twsth,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 9)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twsti,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 10)", GAME_FLAGS, layout_j6twst )
GAMEL( 199?, j6twstj,      j6twst,     impact_nonvideo, j6twst, jpmimpct_state, empty_init, ROT0, "Ace", "Twister (Ace) (IMPACT) (set 11)", GAME_FLAGS, layout_j6twst )

GAME(  199?, j6pompay,     0,          impact_nonvideo, j6pompay, jpmimpct_state, empty_init, ROT0, "Ace", "Up Pompay (Ace) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pompaya,    j6pompay,   impact_nonvideo, j6pompay, jpmimpct_state, empty_init, ROT0, "Ace", "Up Pompay (Ace) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6pompayb,    j6pompay,   impact_nonvideo, j6pompay, jpmimpct_state, empty_init, ROT0, "Ace", "Up Pompay (Ace) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6pompayc,    j6pompay,   impact_nonvideo, j6pompay, jpmimpct_state, empty_init, ROT0, "Ace", "Up Pompay (Ace) (IMPACT) (set 4, Whitbread)", GAME_FLAGS )

GAMEL( 199?, j6vindal,     0,          impact_nonvideo, j6vindal, jpmimpct_state, empty_init, ROT0, "JPM", "Vindaloot (JPM) (IMPACT)", GAME_FLAGS, layout_j6vindal )

GAMEL( 199?, j6vivark,     0,          impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 1)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarka,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 2)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarkb,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 3)", GAME_FLAGS, layout_j6vivark ) // ERROR 71 00
GAMEL( 199?, j6vivarkc,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 4)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarkd,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 5, Whitbread)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarke,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 6)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarkf,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 7)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarkg,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 8)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarkh,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 9)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarki,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 10)", GAME_FLAGS, layout_j6vivark )
GAMEL( 199?, j6vivarkj,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 11)", GAME_FLAGS, layout_j6vivark ) // ERROR 71 00
GAMEL( 199?, j6vivarkk,    j6vivark,   impact_nonvideo, j6vivark, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (JPM) (IMPACT) (set 12)", GAME_FLAGS, layout_j6vivark )

GAME(  199?, j6vivrkc,     0,          impact_nonvideo, j6vivrkc, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (Club) (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6vivrkca,    j6vivrkc,   impact_nonvideo, j6vivrkc, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (Club) (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6vivrkcb,    j6vivrkc,   impact_nonvideo, j6vivrkc, jpmimpct_state, empty_init, ROT0, "JPM", "The Flintstones in Viva Rock Vegas (Club) (JPM) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6knight,     0,          impact_nonvideo, j6knight, jpmimpct_state, empty_init, ROT0, "JPM", "Your Lucky Knight (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6knighta,    j6knight,   impact_nonvideo, j6knight, jpmimpct_state, empty_init, ROT0, "JPM", "Your Lucky Knight (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6knightb,    j6knight,   impact_nonvideo, j6knight, jpmimpct_state, empty_init, ROT0, "JPM", "Your Lucky Knight (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6knightc,    j6knight,   impact_nonvideo, j6knight, jpmimpct_state, empty_init, ROT0, "JPM", "Your Lucky Knight (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6knightd,    j6knight,   impact_nonvideo, j6knight, jpmimpct_state, empty_init, ROT0, "JPM", "Your Lucky Knight (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6knighte,    j6knight,   impact_nonvideo, j6knight, jpmimpct_state, empty_init, ROT0, "JPM", "Your Lucky Knight (JPM) (IMPACT) (set 6)", GAME_FLAGS )

GAMEL( 199?, j6ewn,        0,          impact_nonvideo, j6ewn, jpmimpct_state, empty_init, ROT0, "Crystal", "Each Way Nudger Mark 3 (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6ewn )
GAMEL( 199?, j6ewndg,      j6ewn,      impact_nonvideo, j6ewn, jpmimpct_state, empty_init, ROT0, "Crystal", "Each Way Nudger Mark 3 (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6ewn )
GAMEL( 199?, j6ewndga,     j6ewn,      impact_nonvideo, j6ewn, jpmimpct_state, empty_init, ROT0, "Crystal", "Each Way Nudger Mark 3 (Crystal) (IMPACT) (set 3)", GAME_FLAGS, layout_j6ewn )
GAMEL( 199?, j6ewndgb,     j6ewn,      impact_nonvideo, j6ewn, jpmimpct_state, empty_init, ROT0, "Crystal", "Each Way Nudger Mark 3 (Crystal) (IMPACT) (set 4)", GAME_FLAGS, layout_j6ewn )

GAMEL( 199?, j6btbw,       0,          impact_nonvideo, j6btbw, jpmimpct_state, empty_init, ROT0, "Crystal", "Born To Be Wild Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6btbwd )
GAMEL( 199?, j6btbwa,      j6btbw,     impact_nonvideo, j6btbw, jpmimpct_state, empty_init, ROT0, "Crystal", "Born To Be Wild Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6btbwd )
GAMEL( 199?, j6btbwb,      j6btbw,     impact_nonvideo, j6btbw, jpmimpct_state, empty_init, ROT0, "Crystal", "Born To Be Wild Club (Crystal) (IMPACT) (set 3)", GAME_FLAGS, layout_j6btbwd )
GAMEL( 199?, j6btbwc,      j6btbw,     impact_nonvideo, j6btbw, jpmimpct_state, empty_init, ROT0, "Crystal", "Born To Be Wild Club (Crystal) (IMPACT) (set 4)", GAME_FLAGS, layout_j6btbwd ) // ERROR 71 00
GAMEL( 199?, j6btbwd,      j6btbw,     impact_nonvideo, j6btbw, jpmimpct_state, empty_init, ROT0, "Crystal", "Born To Be Wild Club (Crystal) (IMPACT) (set 5)", GAME_FLAGS, layout_j6btbwd )

GAMEL( 199?, j6cpal,       0,          impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Ace", "Caesars Palace (Ace) (IMPACT) (set 1)", GAME_FLAGS, layout_j6cpal )
GAMEL( 199?, j6cpala,      j6cpal,     impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Ace", "Caesars Palace (Ace) (IMPACT) (set 2)", GAME_FLAGS, layout_j6cpal )
GAMEL( 1998, j6cpalb,      j6cpal,     impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Ace", "Caesars Palace (Ace) (IMPACT) (set 3)", GAME_FLAGS, layout_j6cpal )
GAMEL( 1998, j6cpalc,      j6cpal,     impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Ace", "Caesars Palace (Ace) (IMPACT) (set 4, Whitbread)", GAME_FLAGS, layout_j6cpal )
GAMEL( 1998, j6cpald,      j6cpal,     impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Crystal", "Caesars Palace (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6cpal )
GAMEL( 1998, j6cpale,      j6cpal,     impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Crystal", "Caesars Palace (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6cpal )
GAMEL( 1998, j6cpalf,      j6cpal,     impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Crystal", "Caesars Palace (Crystal) (IMPACT) (set 3)", GAME_FLAGS, layout_j6cpal )
GAMEL( 1998, j6cpalg,      j6cpal,     impact_nonvideo, j6cpal, jpmimpct_state, empty_init, ROT0, "Ace", "Caesars Palace (Ace) (IMPACT) (set 5)", GAME_FLAGS, layout_j6cpal )

GAME(  199?, j6colic,      0,          impact_nonvideo, j6colic, jpmimpct_state, empty_init, ROT0, "Crystal", "Coliseum (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6colica,     j6colic,    impact_nonvideo, j6colic, jpmimpct_state, empty_init, ROT0, "Crystal", "Coliseum (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6colicb,     j6colic,    impact_nonvideo, j6colic, jpmimpct_state, empty_init, ROT0, "Crystal", "Coliseum (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6colicc,     j6colic,    impact_nonvideo, j6colic, jpmimpct_state, empty_init, ROT0, "Crystal", "Coliseum (Crystal) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6colicd,     j6colic,    impact_nonvideo, j6colic, jpmimpct_state, empty_init, ROT0, "Crystal", "Coliseum (Crystal) (IMPACT) (set 5)", GAME_FLAGS )

GAME(  199?, j6easy,       0,          impact_nonvideo, j6easy, jpmimpct_state, empty_init, ROT0, "Crystal", "Easy Money (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6easya,      j6easy,     impact_nonvideo, j6easy, jpmimpct_state, empty_init, ROT0, "Crystal", "Easy Money (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6easyb,      j6easy,     impact_nonvideo, j6easy, jpmimpct_state, empty_init, ROT0, "Crystal", "Easy Money (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6easyc,      j6easy,     impact_nonvideo, j6easy, jpmimpct_state, empty_init, ROT0, "Crystal", "Easy Money (Crystal) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6hdc,        0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6hdca,       j6hdc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6hdcb,       j6hdc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6hdcc,       j6hdc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6hdcd,       j6hdc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6hdce,       j6hdc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 6)", GAME_FLAGS ) // Shows nothing, but code is valid?
GAME(  199?, j6hdcf,       j6hdc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 7)", GAME_FLAGS ) // Shows nothing, but code is valid?
GAME(  199?, j6hdcg,       j6hdc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Hot Dogs Club (Crystal) (IMPACT) (set 8)", GAME_FLAGS ) // Shows nothing, but code is valid?

GAME(  199?, j6kapang,     0,          impact_nonvideo, j6kapang, jpmimpct_state, empty_init, ROT0, "Crystal", "Kapang! (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6kapanga,    j6kapang,   impact_nonvideo, j6kapang, jpmimpct_state, empty_init, ROT0, "Crystal", "Kapang! (Crystal) (IMPACT) (set 2)", GAME_FLAGS )

GAMEL( 199?, j6kfc,        0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Kung Fu Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6kfc )
GAMEL( 199?, j6kfca,       j6kfc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Kung Fu Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6kfc )
GAMEL( 199?, j6kfcb,       j6kfc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Kung Fu Club (Crystal) (IMPACT) (set 3)", GAME_FLAGS, layout_j6kfc )

GAME(  199?, j6lucklo,     0,          impact_nonvideo, j6lucklo, jpmimpct_state, empty_init, ROT0, "Crystal", "Lucky Lottery Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6luckloa,    j6lucklo,   impact_nonvideo, j6lucklo, jpmimpct_state, empty_init, ROT0, "Crystal", "Lucky Lottery Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS )

GAMEL( 199?, j6monst,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Monster Cash Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6monst )
GAMEL( 199?, j6monsta,     j6monst,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Monster Cash Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6monst )
GAMEL( 199?, j6monstb,     j6monst,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Monster Cash Club (Crystal) (IMPACT) (set 3)", GAME_FLAGS, layout_j6monst )
GAMEL( 199?, j6monstc,     j6monst,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Monster Cash Club (Crystal) (IMPACT) (set 4)", GAME_FLAGS, layout_j6monst )
GAMEL( 199?, j6monstd,     j6monst,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Monster Cash Club (Crystal) (IMPACT) (set 5)", GAME_FLAGS, layout_j6monst )

GAME(  199?, j6pinfvr,     0,          impact_nonvideo, j6pinfvr, jpmimpct_state, empty_init, ROT0, "Crystal", "Pinball Fever (Crystal) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6ramese,     0,          impact_nonvideo, j6ramese, jpmimpct_state, empty_init, ROT0, "Crystal", "Rameses' Riches Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6ramesea,    j6ramese,   impact_nonvideo, j6ramese, jpmimpct_state, empty_init, ROT0, "Crystal", "Rameses' Riches Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6rameseb,    j6ramese,   impact_nonvideo, j6ramese, jpmimpct_state, empty_init, ROT0, "Crystal", "Rameses' Riches Club (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6ramesec,    j6ramese,   impact_nonvideo, j6ramese, jpmimpct_state, empty_init, ROT0, "Crystal", "Rameses' Riches Club (Crystal) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6ramesed,    j6ramese,   impact_nonvideo, j6ramese, jpmimpct_state, empty_init, ROT0, "Crystal", "Rameses' Riches Club (Crystal) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6ramesee,    j6ramese,   impact_nonvideo, j6ramese, jpmimpct_state, empty_init, ROT0, "Crystal", "Rameses' Riches Club (Crystal) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6ramesef,    j6ramese,   impact_nonvideo, j6ramese, jpmimpct_state, empty_init, ROT0, "Crystal", "Rameses' Riches Club (Crystal) (IMPACT) (set 7)", GAME_FLAGS )

GAME(  199?, j6r2rum,      0,          impact_nonvideo, j6r2rum, jpmimpct_state, empty_init, ROT0, "Crystal", "Ready To Rumble (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6r2ruma,     j6r2rum,    impact_nonvideo, j6r2rum, jpmimpct_state, empty_init, ROT0, "Crystal", "Ready To Rumble (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6r2rumb,     j6r2rum,    impact_nonvideo, j6r2rum, jpmimpct_state, empty_init, ROT0, "Crystal", "Ready To Rumble (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6r2rumc,     j6r2rum,    impact_nonvideo, j6r2rumc,jpmimpct_state, empty_init, ROT0, "Crystal", "Ready To Rumble (Crystal) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6r2rumd,     j6r2rum,    impact_nonvideo, j6r2rumc,jpmimpct_state, empty_init, ROT0, "Crystal", "Ready To Rumble (Crystal) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6r2rume,     j6r2rum,    impact_nonvideo, j6r2rumc,jpmimpct_state, empty_init, ROT0, "Crystal", "Ready To Rumble (Crystal) (IMPACT) (set 6)", GAME_FLAGS )

GAMEL( 199?, j6slagn,      0,          impact_nonvideo, j6slagn, jpmimpct_state, empty_init, ROT0, "Crystal", "Snakes & Ladders Slides Again (Crystal) (IMPACT) (set 1)", GAME_FLAGS, layout_j6slagng )
GAMEL( 199?, j6slagna,     j6slagn,    impact_nonvideo, j6slagn, jpmimpct_state, empty_init, ROT0, "Crystal", "Snakes & Ladders Slides Again (Crystal) (IMPACT) (set 2)", GAME_FLAGS, layout_j6slagng )
GAMEL( 199?, j6slagnb,     j6slagn,    impact_nonvideo, j6slagn, jpmimpct_state, empty_init, ROT0, "Crystal", "Snakes & Ladders Slides Again (Crystal) (IMPACT) (set 3)", GAME_FLAGS, layout_j6slagng )
GAMEL( 199?, j6slagnc,     j6slagn,    impact_nonvideo, j6slagn, jpmimpct_state, empty_init, ROT0, "Crystal", "Snakes & Ladders Slides Again (Crystal) (IMPACT) (set 4)", GAME_FLAGS, layout_j6slagng )

GAME(  199?, j6tqust,      0,          impact_nonvideo, j6tqust, jpmimpct_state, empty_init, ROT0, "Crystal", "Treasure Quest (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6tqusta,     j6tqust,    impact_nonvideo, j6tqust, jpmimpct_state, empty_init, ROT0, "Crystal", "Treasure Quest (Crystal) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6tqustb,     j6tqust,    impact_nonvideo, j6tqust, jpmimpct_state, empty_init, ROT0, "Crystal", "Treasure Quest (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6tqustc,     j6tqust,    impact_nonvideo, j6tqust, jpmimpct_state, empty_init, ROT0, "Crystal", "Treasure Quest (Crystal) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6shoot,      0,          impact_nonvideo, j6shoot, jpmimpct_state, empty_init, ROT0, "JPM", "ShootOut (JPM) (IMPACT) (Whitbread)", GAME_FLAGS )


// *************************************************************************************************************
// Mdm type games
// *************************************************************************************************************

GAMEL( 199?, j6amdrm,      0,          impact_nonvideo, j6amdrm, jpmimpct_state, empty_init, ROT0, "Mdm", "American Dream (Mdm) (IMPACT)", GAME_FLAGS, layout_j6amdrm )

GAME(  199?, j6col,        0,          impact_nonvideo, j6col, jpmimpct_state, empty_init, ROT0, "Mdm", "Coliseum (Mdm) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cola,       j6col,      impact_nonvideo, j6col, jpmimpct_state, empty_init, ROT0, "Mdm", "Coliseum (Mdm) (IMPACT) (set 2)", GAME_FLAGS )

// *************************************************************************************************************
// Empire type games 'No Binary Mech'
// *************************************************************************************************************

GAME(  199?, j6bigcsh,     0,          impact_nonvideo, j6bigcsh, jpmimpct_state, empty_init, ROT0, "Empire", "Big Cash Machine (Empire) (IMPACT)", GAME_FLAGS )

// parent is ERROR 6.0, others are No Binary Mech
GAME(  199?, j6cshbox,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Cash Box Club (Empire) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cshboxa,    j6cshbox,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Cash Box Club (Empire) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6cshboxb,    j6cshbox,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Cash Box Club (Empire) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6cshbeu,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Cash Box Club (Empire) (Euro) (IMPACT)", GAME_FLAGS )

// boots after initial reset
GAME(  199?, j6gldmin,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Gold Mine (Empire) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6papa,       0,          impact_nonvideo, j6papa, jpmimpct_state, empty_init, ROT0, "Empire", "Paparazzi (Empire) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6papaa,      j6papa,     impact_nonvideo, j6papa, jpmimpct_state, empty_init, ROT0, "Empire", "Paparazzi (Empire) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6papab,      j6papa,     impact_nonvideo, j6papa, jpmimpct_state, empty_init, ROT0, "Empire", "Paparazzi (Empire) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6papac,      j6papa,     impact_nonvideo, j6papa, jpmimpct_state, empty_init, ROT0, "Empire", "Paparazzi (Empire) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6papad,      j6papa,     impact_nonvideo, j6papa, jpmimpct_state, empty_init, ROT0, "Empire", "Paparazzi (Empire) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6papae,      j6papa,     impact_nonvideo, j6papa, jpmimpct_state, empty_init, ROT0, "Empire", "Paparazzi (Empire) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6papaf,      j6papa,     impact_nonvideo, j6papa, jpmimpct_state, empty_init, ROT0, "Empire", "Paparazzi (Empire) (IMPACT) (set 7)", GAME_FLAGS )

GAME(  199?, j6wthing,     0,          impact_nonvideo, j6wthing, jpmimpct_state, empty_init, ROT0, "Empire", "Wild Thing (Empire) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6wthinga,    j6wthing,   impact_nonvideo, j6wthing, jpmimpct_state, empty_init, ROT0, "Empire", "Wild Thing (Empire) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6wthingb,    j6wthing,   impact_nonvideo, j6wthing, jpmimpct_state, empty_init, ROT0, "Empire", "Wild Thing (Empire) (IMPACT) (set 3)", GAME_FLAGS )

// always in freeplay
GAME(  199?, j6hisprt,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "High Spirits (Empire) (IMPACT) (prototype?)", GAME_FLAGS ) // roms are marked 'DEMO'

// corrupt VFD (different type, or hooked up to something else?)
GAME(  199?, j6footy,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Football Fever (Empire) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6footya,     j6footy,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Football Fever (Empire) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6footyb,     j6footy,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Empire", "Football Fever (Empire) (IMPACT) (set 3)", GAME_FLAGS )

// *************************************************************************************************************
// Games with no VFD output
// *************************************************************************************************************

GAME(  199?, j6crsfir,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Cross Fire (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6crsfira,    j6crsfir,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Cross Fire (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6daygld,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Gold (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6dayglda,    j6daygld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Gold (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6daygldb,    j6daygld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Gold (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6daygldc,    j6daygld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Gold (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6daygldd,    j6daygld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Gold (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6dayglde,    j6daygld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Gold (JPM) (IMPACT) (set 6)", GAME_FLAGS )

GAME(  199?, j6dayml,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Millennium (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6daymla,     j6dayml,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Millennium (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6daymlb,     j6dayml,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Daytona Millennium (JPM) (IMPACT) (set 3)", GAME_FLAGS )

GAME(  199?, j6dmnjkr,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Demon Jokers (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6dmnjkra,    j6dmnjkr,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Demon Jokers (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6dmnjkrb,    j6dmnjkr,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Demon Jokers (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6dmnjkrc,    j6dmnjkr,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Demon Jokers (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6reddmn,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Red Demon (JPM) (IMPACT)", GAME_FLAGS ) // the same as Demon Jokers but with a different sound rom?
GAME(  199?, j6gldday,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Day (JPM) (IMPACT)", GAME_FLAGS ) // this is the same as j6dmnjkra

GAME(  199?, j6brkout,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Breakout (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6brkouta,    j6brkout,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Breakout (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6cshcnt,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Countdown (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6cshcnta,    j6cshcnt,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Cash Countdown (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6golddm,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Demons (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6golddma,    j6golddm,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Golden Demons (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6jkrpls,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6jkrplsa,    j6jkrpls,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6jkrplsb,    j6jkrpls,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6jkrplsc,    j6jkrpls,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6jkrplsd,    j6jkrpls,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6jkrplse,    j6jkrpls,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus (JPM) (IMPACT) (set 6)", GAME_FLAGS )

GAME(  199?, j6jkpldx,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus Deluxe (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6jkpldxa,    j6jkpldx,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Plus Deluxe (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6phxgld,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6phxglda,    j6phxgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6phxgldb,    j6phxgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6phxgldc,    j6phxgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 4)", GAME_FLAGS )
GAME(  199?, j6phxgldd,    j6phxgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 5)", GAME_FLAGS )
GAME(  199?, j6phxglde,    j6phxgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 6)", GAME_FLAGS )
GAME(  199?, j6phxgldf,    j6phxgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 7)", GAME_FLAGS )
GAME(  199?, j6phxgldg,    j6phxgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold (JPM) (IMPACT) (set 8)", GAME_FLAGS )

GAME(  199?, j6pnxgd,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Gold De Luxe (JPM) (IMPACT)", GAME_FLAGS )

GAME(  199?, j6pnxmil,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Millennium (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pnxmila,    j6pnxmil,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Millennium (JPM) (IMPACT) (set 2)", GAME_FLAGS )
GAME(  199?, j6pnxmilb,    j6pnxmil,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Millennium (JPM) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6pnxmilc,    j6pnxmil,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Phoenix Millennium (JPM) (IMPACT) (set 4)", GAME_FLAGS )

// some text strings?
GAME(  199?, j6pwrlin,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Power Lines (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pwrlina,    j6pwrlin,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Power Lines (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6bmc,        0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Big Money Club (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6bmca,       j6bmc,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Big Money Club (Crystal) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6bno,        0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Big Nite Out (Crystal) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6bnoa,       j6bno,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Big Nite Out (Crystal) (IMPACT) (set 2)", GAME_FLAGS ) // code crashes
GAME(  199?, j6bnob,       j6bno,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Big Nite Out (Crystal) (IMPACT) (set 3)", GAME_FLAGS )
GAME(  199?, j6bnoc,       j6bno,      impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Crystal", "Big Nite Out (Crystal) (IMPACT) (set 4)", GAME_FLAGS )

GAME(  199?, j6svndb,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Ace", "7 Deadly Bins (Ace) (IMPACT)", GAME_FLAGS )

// *************************************************************************************************************
// Unusual cases
// *************************************************************************************************************

// accesses 4801e0 (2nd duart?)
GAME(  199?, j6fifth,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "Ace", "5th Dimension (Ace) (IMPACT)", GAME_FLAGS )

// shows 'IMPACT'
GAMEL( 199?, j6indyge,     j6indy,     impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT, German set 1)", GAME_FLAGS, layout_j6indy8 ) // was marked as Sys5 'Indy 500' but is clearly IMPACT and has Indiana Jones / Holy Grail strings (might be a different game to the english set)
GAMEL( 199?, j6indyge2,    j6indy,     impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Indiana Jones (JPM) (IMPACT, German set 2)", GAME_FLAGS, layout_j6indy8 )

// shows 'IMPACT'
GAME(  199?, j6jkrgld,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Gold (JPM) (IMPACT)", GAME_FLAGS )

// shows 'IMPACT' but boots
// shows LJP - FORTUNA (Linked JackPot?) also has Barcrest strings? shows JPM website address in demo
GAME(  199?, j6jkwld,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Jokers Wild (JPM) (IMPACT)", GAME_FLAGS )

// similar to j6jkwld
GAME(  199?, j6kamel,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Kameleon (JPM) (IMPACT)", GAME_FLAGS )

// similar to j6jkwld
GAME(  199?, j6twstdt,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Twister (JPM) [Dutch] (IMPACT)", GAME_FLAGS )

// just shows the software version?
GAME(  199?, j6milln,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Millionaire (JPM) (IMPACT) (set 1)", GAME_FLAGS ) // was marked as SWP, should this be a video game? - Apparently it's just a link box using the same hardware, but for 3 PC based units which aren't dumped
GAME(  199?, j6millna,     j6milln,    impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Millionaire (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6pirgld,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Pirates Gold (JPM) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6pirglda,    j6pirgld,   impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Pirates Gold (JPM) (IMPACT) (set 2)", GAME_FLAGS )

GAME(  199?, j6rager,      0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Red Alert (JPM) [German] (IMPACT)", GAME_FLAGS )

// resets repeatedly, appears to access a 2nd Duart, has a 'Crane' feature (so probably failing for that reason?)
// demo sequence has 'Showcase Ltd' possibly Showcase Cinema related?
GAME(  199?, j6start,      0,          impact_nonvideo, j6start, jpmimpct_state, empty_init, ROT0, "Showcase Ltd", "Starturn (JPM / Showcase) (IMPACT) (set 1)", GAME_FLAGS )
GAME(  199?, j6starta,     j6start,    impact_nonvideo, j6start, jpmimpct_state, empty_init, ROT0, "Showcase Ltd", "Starturn (JPM / Showcase) (IMPACT) (set 2)", GAME_FLAGS )

// does have text strings so should display something?
// sets stack to 40a000 which is outside normal memory range, is this in the incorrect driver?
GAME(  199?, j6maxcsh,     0,          impact_nonvideo, jpmimpct_non_video_inputs, jpmimpct_state, empty_init, ROT0, "JPM", "Maximus Cash (JPM) (IMPACT)", GAME_FLAGS )
