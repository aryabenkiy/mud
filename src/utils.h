/* ************************************************************************
*   File: utils.h                                       Part of Bylins    *
*  Usage: header file: utility macros and prototypes of utility funcs     *                                                                         *
*  All rights reserved.  See license.doc for complete information.        *
*                                                                         *
*  Copyright (C) 1993, 94 by the Trustees of the Johns Hopkins University *
*  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
*                                                                         *
*  $Author$                                                        *
*  $Date$                                           *
*  $Revision$                                                       *
************************************************************************ */

#ifndef _UTILS_H_
#define _UTILS_H_

#include "config.hpp"
#include "structs.h"
#include "conf.h"
#include "pugixml.hpp"

#include <boost/dynamic_bitset.hpp>

#include <string>
#include <list>
#include <new>
#include <vector>
#include <sstream>

struct ROOM_DATA;	// forward declaration to avoid inclusion of room.hpp and any dependencies of that header.
class CHAR_DATA;	// forward declaration to avoid inclusion of char.hpp and any dependencies of that header.

// external declarations and prototypes *********************************

#define not_null(ptr, str) (ptr && *ptr) ? ptr : str ? str : "undefined"

inline const char* not_empty(const std::string& s)
{
	return s.empty() ? "undefined" : s.c_str();
}

inline const char* not_empty(const std::string& s, const char* subst)
{
	return s.empty()
		? (subst
			? subst
			: "undefined")
		: s.c_str();
}

extern struct weather_data weather_info;
extern char AltToKoi[];
extern char KoiToAlt[];
extern char WinToKoi[];
extern char KoiToWin[];
extern char KoiToWin2[];
extern char AltToLat[];

extern int class_stats_limit[NUM_PLAYER_CLASSES][6];

// public functions in utils.cpp
CHAR_DATA *find_char(long n);
char *rustime(const struct tm *timeptr);
char *str_dup(const char *source);
char *str_add(char *dst, const char *src);
int str_cmp(const char *arg1, const char *arg2);
int str_cmp(const std::string &arg1, const char *arg2);
int str_cmp(const char *arg1, const std::string &arg2);
int str_cmp(const std::string &arg1, const std::string &arg2);
int strn_cmp(const char *arg1, const char *arg2, size_t n);
int strn_cmp(const std::string &arg1, const char *arg2, size_t n);
int strn_cmp(const char *arg1, const std::string &arg2, size_t n);
int strn_cmp(const std::string &arg1, const std::string &arg2, size_t n);
int touch(const char *path);
void pers_log(CHAR_DATA *ch, const char *format, ...);
int number(int from, int to);
int dice(int number, int size);
void sprinttype(int type, const char *names[], char *result);
int get_line(FILE * fl, char *buf);
int get_filename(const char *orig_name, char *filename, int mode);
TIME_INFO_DATA *age(CHAR_DATA * ch);
int num_pc_in_room(ROOM_DATA * room);
void core_dump_real(const char *, int);
int replace_str(const AbstractStringWriter::shared_ptr& writer, const char *pattern, const char *replacement, int rep_all, int max_size);
void format_text(const AbstractStringWriter::shared_ptr& writer, int mode, DESCRIPTOR_DATA * d, size_t maxlen);
int check_moves(CHAR_DATA * ch, int how_moves);
void to_koi(char *str, int from);
void from_koi(char *str, int to);
void koi_to_alt(char *str, int len);
void koi_to_win(char *str, int len);
void koi_to_winz(char *str, int len);
#ifdef HAVE_ICONV
void koi_to_utf8(char *str_i, char *str_o);
void utf8_to_koi(char *str_i, char *str_o);
#endif
int real_sector(int room);
char *format_act(const char *orig, CHAR_DATA * ch, OBJ_DATA * obj, const void *vict_obj);
int roundup(float fl);
int valid_email(const char *address);
void skip_dots(char **string);
const char * str_str(const char *cs, const char *ct);
void kill_ems(char *str);
bool die_follower(CHAR_DATA * ch);
void cut_one_word(std::string &str, std::string &word);
size_t strl_cpy(char *dst, const char *src, size_t siz);
int get_real_dr(CHAR_DATA *ch);
extern bool GetAffectNumByName(const std::string& affName, EAffectFlag& result);
void tell_to_char(CHAR_DATA *keeper, CHAR_DATA *ch, const char *arg);
bool is_head(std::string name);
extern std::list<FILE *> opened_files;
extern bool is_dark(room_rnum room);
#define core_dump()     core_dump_real(__FILE__, __LINE__)
extern const char *ACTNULL;

#define CHECK_NULL(pointer, expression) \
  if ((pointer) == NULL) i = ACTNULL; else i = (expression);

#define MIN_TITLE_LEV   25

// undefine MAX and MIN so that our functions are used instead
#ifdef MAX
#undef MAX
#endif

#ifdef MIN
#undef MIN
#endif

#define SF_EMPTY       (1 << 0)
#define SF_FOLLOWERDIE (1 << 1)
#define SF_MASTERDIE   (1 << 2)
#define SF_CHARMLOST   (1 << 3)
#define SF_SILENCE     (1 << 4)

#define EXP_IMPL 2000000000
#define MAX_AUCTION_LOT            3
#define MAX_AUCTION_TACT_BUY       5
#define MAX_AUCTION_TACT_PRESENT   (MAX_AUCTION_TACT_BUY + 3)
#define AUCTION_PULSES             30
#define CHAR_DRUNKED               10
#define CHAR_MORTALLY_DRUNKED      16
#define MAX_COND_VALUE			   24

int MAX(int a, int b);
int MIN(int a, int b);
// all argument min/max macros definition
#define MMIN(a,b) ((a<b)?a:b)
#define MMAX(a,b) ((a<b)?b:a)

char *colorLOW(char *txt);
char * colorCAP(char *txt);
char * CAP(char *txt);

#define KtoW(c) ((ubyte)(c) < 128 ? (c) : KoiToWin[(ubyte)(c)-128])
#define KtoW2(c) ((ubyte)(c) < 128 ? (c) : KoiToWin2[(ubyte)(c)-128])
#define KtoA(c) ((ubyte)(c) < 128 ? (c) : KoiToAlt[(ubyte)(c)-128])
#define WtoK(c) ((ubyte)(c) < 128 ? (c) : WinToKoi[(ubyte)(c)-128])
#define AtoK(c) ((ubyte)(c) < 128 ? (c) : AltToKoi[(ubyte)(c)-128])
#define AtoL(c) ((ubyte)(c) < 128 ? (c) : AltToLat[(ubyte)(c)-128])

// in magic.cpp //
bool circle_follow(CHAR_DATA * ch, CHAR_DATA * victim);

// in act.informative.cpp //
void look_at_room(CHAR_DATA * ch, int mode);

// in act.movmement.cpp //
int do_simple_move(CHAR_DATA * ch, int dir, int following, CHAR_DATA * leader);
int perform_move(CHAR_DATA * ch, int dir, int following, int checkmob, CHAR_DATA * leader);

// in limits.cpp //
int mana_gain(CHAR_DATA * ch);
int hit_gain(CHAR_DATA * ch);
int move_gain(CHAR_DATA * ch);
void advance_level(CHAR_DATA * ch);
void gain_exp(CHAR_DATA * ch, int gain);
void gain_exp_regardless(CHAR_DATA * ch, int gain);
void gain_condition(CHAR_DATA * ch, unsigned condition, int value);
void check_idling(CHAR_DATA * ch);
void point_update(void);
void room_point_update();
void exchange_point_update();
void obj_point_update();
void update_pos(CHAR_DATA * victim);

// various constants ****************************************************

// ���������, ����� �� �������� ����� �� ����
bool check_spell_on_player(CHAR_DATA *ch, int spell_num);


// get_filename() //
#define ALIAS_FILE        1
#define SCRIPT_VARS_FILE  2
#define PLAYERS_FILE      3
#define TEXT_CRASH_FILE   4
#define TIME_CRASH_FILE   5
#define PERS_DEPOT_FILE   6
#define SHARE_DEPOT_FILE  7
#define PURGE_DEPOT_FILE  8

// breadth-first searching //
#define BFS_ERROR        -1
#define BFS_ALREADY_THERE  -2
#define BFS_NO_PATH         -3

/*
 * XXX: These constants should be configurable. See act.informative.c
 * and utils.cpp for other places to change.
 */
// mud-life time
#define HOURS_PER_DAY          24
#define DAYS_PER_MONTH         30
#define MONTHS_PER_YEAR        12
#define SECS_PER_PLAYER_AFFECT 2
#define SECS_PER_ROOM_AFFECT 2
#define TIME_KOEFF             2
#define MOB_MEM_KOEFF          SECS_PER_MUD_HOUR
#define SECS_PER_MUD_HOUR     60
#define SECS_PER_MUD_DAY      (HOURS_PER_DAY*SECS_PER_MUD_HOUR)
#define SECS_PER_MUD_MONTH    (DAYS_PER_MONTH*SECS_PER_MUD_DAY)
#define SECS_PER_MUD_YEAR     (MONTHS_PER_YEAR*SECS_PER_MUD_MONTH)
#define NEWMOONSTART               27
#define NEWMOONSTOP                1
#define HALFMOONSTART              7
#define FULLMOONSTART              13
#define FULLMOONSTOP               15
#define LASTHALFMOONSTART          21
#define MOON_CYCLE               28
#define WEEK_CYCLE               7
#define POLY_WEEK_CYCLE          9

// real-life time (remember Real Life?)
#define SECS_PER_REAL_MIN  60
#define SECS_PER_REAL_HOUR (60*SECS_PER_REAL_MIN)
#define SECS_PER_REAL_DAY  (24*SECS_PER_REAL_HOUR)
#define SECS_PER_REAL_YEAR (365*SECS_PER_REAL_DAY)

#define IS_IMMORTAL(ch)     (!IS_NPC(ch) && GET_LEVEL(ch) >= LVL_IMMORT)
#define IS_GOD(ch)          (!IS_NPC(ch) && GET_LEVEL(ch) >= LVL_GOD)
#define IS_GRGOD(ch)        (!IS_NPC(ch) && GET_LEVEL(ch) >= LVL_GRGOD)
#define IS_IMPL(ch)         (!IS_NPC(ch) && GET_LEVEL(ch) >= LVL_IMPL)
#define IS_HIGHGOD(ch)      (IS_IMPL(ch) && (GET_GOD_FLAG(ch,GF_HIGHGOD)))

#define IS_BITS(mask, bitno) (IS_SET(mask,(1 << bitno)))
#define IS_CASTER(ch)        (IS_BITS(MASK_CASTER,GET_CLASS(ch)))
#define IS_MAGE(ch)          (IS_BITS(MASK_MAGES, GET_CLASS(ch)))

extern int receptionist(CHAR_DATA*, void*, int, char*);
extern int postmaster(CHAR_DATA*, void*, int, char*);
extern int bank(CHAR_DATA*, void*, int, char*);
extern int shop_ext(CHAR_DATA*, void*, int, char*);

#define IS_SHOPKEEPER(ch) (IS_MOB(ch) && mob_index[GET_MOB_RNUM(ch)].func == shop_ext)
#define IS_RENTKEEPER(ch) (IS_MOB(ch) && mob_index[GET_MOB_RNUM(ch)].func == receptionist)
#define IS_POSTKEEPER(ch) (IS_MOB(ch) && mob_index[GET_MOB_RNUM(ch)].func == postmaster)
#define IS_BANKKEEPER(ch) (IS_MOB(ch) && mob_index[GET_MOB_RNUM(ch)].func == bank)

// string utils *********************************************************


#define YESNO(a) ((a) ? "YES" : "NO")
#define ONOFF(a) ((a) ? "ON" : "OFF")

#define LOWER(c)   (a_lcc(c))
#define UPPER(c)   (a_ucc(c))
#define LATIN(c)   (a_lat(c))

#define ISNEWL(ch) ((ch) == '\n' || (ch) == '\r')
#define IF_STR(st) ((st) ? (st) : "\0")

// memory utils *********************************************************

template <typename T>
inline void CREATE(T*& result, const size_t number)
{
	result = static_cast<T*>(calloc(number, sizeof(T)));
	if (!result)
	{
		perror("SYSERR: calloc failure");
		abort();
	}
}

template <> inline void CREATE(EXTRA_DESCR_DATA*&, const size_t)
{
	throw std::runtime_error("for EXTRA_DESCR_DATA you have to use operator new");
}

template <typename T>
inline void RECREATE(T*& result, const size_t number)
{
	result = static_cast<T*>(realloc(result, number*sizeof(T)));
	if (!result)
	{
		perror("SYSERR: realloc failure");
		abort();
	}
}

template <typename T>
inline T* NEWCREATE()
{
	T* result = new(std::nothrow) T;
	if (!result)
	{
		perror("SYSERR: new failure");
		abort();
	}
	return result;
}

template <typename T>
inline void NEWCREATE(T*& result)
{
	result = NEWCREATE<T>();
}

template <typename T, typename I>
inline void NEWCREATE(T*& result, const I& init_value)
{
	result = new(std::nothrow) T(init_value);
	if (!result)
	{
		perror("SYSERR: new failure");
		abort();
	}
}

/*
 * the source previously used the same code in many places to remove an item
 * from a list: if it's the list head, change the head, else traverse the
 * list looking for the item before the one to be removed.  Now, we have a
 * macro to do this.  To use, just make sure that there is a variable 'temp'
 * declared as the same type as the list to be manipulated.  BTW, this is
 * a great application for C++ templates but, alas, this is not C++.  Maybe
 * CircleMUD 4.0 will be...
 */
template <typename ListType, typename GetNextFunc>
inline void REMOVE_FROM_LIST(ListType* item, ListType*& head, GetNextFunc next)
{
	if ((item) == (head))
	{
		head = next(item);
	}
	else
	{
		auto temp = head;
		while (temp && (next(temp) != (item)))
		{
			temp = next(temp);
		}
		if (temp)
		{
			next(temp) = next(item);
		}
	}
}

template <typename ListType>
inline void REMOVE_FROM_LIST(ListType* item, ListType*& head)
{
	REMOVE_FROM_LIST(item, head, [](ListType* list) -> ListType*& { return list->next; });
}

// basic bitvector utils ************************************************

template <typename T> struct UNIMPLEMENTED { };

template <typename T>
inline bool IS_SET(const T flag, const uint32_t bit)
{
	return 0 != (flag & 0x3FFFFFFF & bit);
}

template <typename T, typename EnumType>
inline void SET_BIT(T& var, const EnumType bit)
{
	var |= (to_underlying(bit) & 0x3FFFFFFF);
}

template <typename T>
inline void SET_BIT(T& var, const uint32_t bit)
{
	var |= (bit & 0x3FFFFFFF);
}

template <typename T>
inline void SET_BIT(T& var, const int bit)
{
	var |= (bit & 0x3FFFFFFF);
}

template <typename T, typename EnumType>
inline void REMOVE_BIT(T& var, const EnumType bit)
{
	var &= ~(to_underlying(bit) & 0x3FFFFFFF);
}

template <typename T>
inline void REMOVE_BIT(T& var, const uint32_t bit)
{
	var &= ~(bit & 0x3FFFFFFF);
}

template <typename T>
inline void REMOVE_BIT(T& var, const int bit)
{
	var &= ~(bit & 0x3FFFFFFF);
}

template <typename T>
inline void TOGGLE_BIT(T& var, const uint32_t bit)
{
	var = var ^ (bit & 0x3FFFFFFF);
}

// TODO: Get rid of me.
#define CHECK_PLAYER_SPECIAL(ch, var)  (var)

#define MOB_FLAGS(ch)  ((ch)->char_specials.saved.act)
#define PLR_FLAGS(ch)  ((ch)->char_specials.saved.act)
#define PRF_FLAGS(ch)  ((ch)->player_specials->saved.pref)
#define NPC_FLAGS(ch)  ((ch)->mob_specials.npc_flags)
#define ROOM_AFF_FLAGS(room)  ((room)->affected_by)
#define EXTRA_FLAGS(ch) ((ch)->Temporary)
#define GET_ROOM(loc) (world[(loc)])
#define DESC_FLAGS(d)   ((d)->options)
#define SPELL_ROUTINES(spl) (spell_info[spl].routines)

// See http://www.circlemud.org/~greerga/todo.009 to eliminate MOB_ISNPC.
#define IS_NPC(ch)           (MOB_FLAGS(ch).get(MOB_ISNPC))
#define IS_MOB(ch)          (IS_NPC(ch) && GET_MOB_RNUM(ch) >= 0)

#define MOB_FLAGGED(ch, flag)   (IS_NPC(ch) && MOB_FLAGS(ch).get(flag))
#define PLR_FLAGGED(ch, flag)   (!IS_NPC(ch) && PLR_FLAGS(ch).get(flag))
#define PRF_FLAGGED(ch, flag)   (PRF_FLAGS(ch).get(flag))
#define NPC_FLAGGED(ch, flag)   (NPC_FLAGS(ch).get(flag))
#define EXTRA_FLAGGED(ch, flag) (EXTRA_FLAGS(ch).get(flag))
#define ROOM_FLAGGED(loc, flag) (GET_ROOM((loc))->get_flag(flag))
#define ROOM_AFFECTED(loc, flag) (ROOM_AFF_FLAGS((world[(loc)])).get(flag))
#define EXIT_FLAGGED(exit, flag)     (IS_SET((exit)->exit_info, (flag)))
#define OBJVAL_FLAGGED(obj, flag)    (IS_SET(GET_OBJ_VAL((obj), 1), (flag)))
#define OBJWEAR_FLAGGED(obj, mask)   (obj->get_wear_mask(mask))
#define DESC_FLAGGED(d, flag) (IS_SET(DESC_FLAGS(d), (flag)))
#define HAS_SPELL_ROUTINE(spl, flag) (IS_SET(SPELL_ROUTINES(spl), (flag)))

// IS_AFFECTED for backwards compatibility
#define IS_AFFECTED(ch, skill) (AFF_FLAGGED(ch, EAffectFlag::skill))

#define PLR_TOG_CHK(ch, flag) (PLR_FLAGS(ch).toggle(flag))
#define PRF_TOG_CHK(ch, flag) (PRF_FLAGS(ch).toggle(flag))

// room utils ***********************************************************
#define SECT(room)   (world[(room)]->sector_type)
#define GET_ROOM_SKY(room) (world[room]->weather.duration > 0 ? \
                            world[room]->weather.sky : weather_info.sky)

#define IS_MOONLIGHT(room) ((GET_ROOM_SKY(room) == SKY_LIGHTNING && \
                             weather_info.moon_day >= FULLMOONSTART && \
                             weather_info.moon_day <= FULLMOONSTOP))

#define IS_TIMEDARK(room)  ((world[room]->gdark > world[room]->glight) || \
                            (!(world[room]->light+world[room]->fires+world[room]->glight) && \
                              (ROOM_FLAGGED(room, ROOM_DARK) || \
                              (SECT(room) != SECT_INSIDE && \
                               SECT(room) != SECT_CITY   && \
                               ( weather_info.sunlight == SUN_SET || \
                                 weather_info.sunlight == SUN_DARK )) ) ) )

#define IS_DEFAULTDARK(room) (ROOM_FLAGGED(room, ROOM_DARK) || \
                              (SECT(room) != SECT_INSIDE && \
                               SECT(room) != SECT_CITY   && \
                               ( weather_info.sunlight == SUN_SET || \
                                 weather_info.sunlight == SUN_DARK )) )

#define IS_DARK(room) is_dark(room)

#define IS_DARKOLD(room)      ((world[room]->gdark > (world[room]->glight + world[room]->light + world[room]->fires)) || \
                            (!(world[room]->gdark < (world[room]->glight + world[room]->light + world[room]->fires)) && \
                              !ROOM_AFFECTED(room, AFF_ROOM_LIGHT) && \
				(ROOM_FLAGGED(room, ROOM_DARK) || \
                              (SECT(room) != SECT_INSIDE && \
                               SECT(room) != SECT_CITY   && \
                               ( weather_info.sunlight == SUN_SET || \
                                 (weather_info.sunlight == SUN_DARK && \
                                  !IS_MOONLIGHT(room)) )) ) ) )

#define IS_DARKTHIS(room)      ((world[room]->gdark > world[room]->glight) || \
                                (!(world[room]->gdark < world[room]->glight) && \
                                 !(world[room]->light+world[room]->fires) && \
                                  (ROOM_FLAGGED(room, ROOM_DARK) || \
                                  (SECT(room) != SECT_INSIDE && \
                                   SECT(room) != SECT_CITY   && \
                                   ( weather_info.sunlight == SUN_DARK && \
                                     !IS_MOONLIGHT(room) )) ) ) )

#define IS_DARKSIDE(room)      ((world[room]->gdark > world[room]->glight) || \
                                (!(world[room]->gdark < world[room]->glight) && \
                                 !(world[room]->light+world[room]->fires) && \
                                  (ROOM_FLAGGED(room, ROOM_DARK) || \
                                  (SECT(room) != SECT_INSIDE && \
                                   SECT(room) != SECT_CITY   && \
                                   ( weather_info.sunlight == SUN_SET  || \
                                     weather_info.sunlight == SUN_RISE || \
                                     (weather_info.sunlight == SUN_DARK && \
                                      !IS_MOONLIGHT(room) )) ) ) )

#define IS_LIGHT(room)     (!IS_DARK(room))

#define VALID_RNUM(rnum)   ((rnum) > 0 && (rnum) <= top_of_world)
#define GET_ROOM_VNUM(rnum) ((room_vnum)(VALID_RNUM(rnum) ? world[(rnum)]->number : NOWHERE))
#define GET_ROOM_SPEC(room) (VALID_RNUM(room) ? world[(room)]->func : NULL)

// char utils ***********************************************************
#define IS_MANA_CASTER(ch) (GET_CLASS(ch)==CLASS_DRUID)
#define IN_ROOM(ch)  ((ch)->in_room)
#define GET_AGE(ch)     (age(ch)->year)
#define GET_REAL_AGE(ch) (age(ch)->year + GET_AGE_ADD(ch))
#define GET_PC_NAME(ch) ((ch)->get_pc_name().c_str())
#define GET_NAME(ch)    ((ch)->get_name().c_str())
#define GET_HELPER(ch)  ((ch)->helpers)
#define GET_TITLE(ch)   ((ch)->player_data.title)
#define GET_LEVEL(ch)   ((ch)->get_level())
#define GET_MAX_MANA(ch)      MIN(9998,mana[MIN(50, (int) GET_REAL_WIS(ch))]+GET_REMORT(ch)*500)
//#define GET_MANA_COST(ch,spellnum)      (mana_cost_cs[(int)GET_LEVEL(ch)][spell_create[spellnum].runes.krug-1])
#define GET_MANA_COST(ch,spellnum)      mag_manacost(ch,spellnum)
#define GET_MANA_STORED(ch)   ((ch)->MemQueue.stored)
#define GET_MEM_COMPLETED(ch) ((ch)->MemQueue.stored)
#define GET_MEM_CURRENT(ch)   (MEMQUEUE_EMPTY(ch)?0:mag_manacost(ch,(ch)->MemQueue.queue->spellnum))
#define GET_MEM_TOTAL(ch)     ((ch)->MemQueue.total)
#define MEMQUEUE_EMPTY(ch)    ((ch)->MemQueue.queue == NULL)
#define INITIATIVE(ch)        ((ch)->Initiative)
#define BATTLECNTR(ch)        ((ch)->BattleCounter)
#define ROUND_COUNTER(ch)     ((ch)->round_counter)
#define EXTRACT_TIMER(ch)     ((ch)->ExtractTimer)
#define CHECK_AGRO(ch)        ((ch)->CheckAggressive)
#define WAITLESS(ch)          (IS_IMMORTAL(ch))
#define PUNCTUAL_WAITLESS(ch)          (IS_IMMORTAL(ch) || GET_GOD_FLAG(ch, GF_GODSLIKE))
#define CLR_MEMORY(ch)  (memset((ch)->Memory,0,MAX_SPELLS+1))
#define IS_CODER(ch)    (GET_LEVEL(ch) < LVL_IMMORT && PRF_FLAGGED(ch, PRF_CODERINFO))
#define IS_COLORED(ch)    (pk_count (ch))
#define MAX_PORTALS(ch)  ((GET_LEVEL(ch)/3)+GET_REMORT(ch))

#define GET_AF_BATTLE(ch,flag) ((ch)->BattleAffects.get(flag))
#define SET_AF_BATTLE(ch,flag) ((ch)->BattleAffects.set(flag))
#define CLR_AF_BATTLE(ch,flag) ((ch)->BattleAffects.unset(flag))
#define NUL_AF_BATTLE(ch)      ((ch)->BattleAffects.clear())
#define GET_REMORT(ch)         ((ch)->get_remort())
#define GET_SKILL(ch, skill)   ((ch)->get_skill(skill))
#define GET_EMAIL(ch)          ((ch)->player_specials->saved.EMail)
#define GET_LASTIP(ch)         ((ch)->player_specials->saved.LastIP)
#define GET_GOD_FLAG(ch,flag)  (IS_SET((ch)->player_specials->saved.GodsLike,flag))
#define SET_GOD_FLAG(ch,flag)  (SET_BIT((ch)->player_specials->saved.GodsLike,flag))
#define CLR_GOD_FLAG(ch,flag)  (REMOVE_BIT((ch)->player_specials->saved.GodsLike,flag))
#define GET_UNIQUE(ch)         ((ch)->get_uid())
#define LAST_LOGON(ch)         ((ch)->get_last_logon())
#define LAST_EXCHANGE(ch)         ((ch)->get_last_exchange())
//��������� ��� �������� ���������� ����� �� ����� (�) ��������
#define GET_RIP_ARENA(ch)      ((ch)->player_specials->saved.Rip_arena)
#define GET_RIP_PK(ch)         ((ch)->player_specials->saved.Rip_pk)
#define GET_RIP_MOB(ch)        ((ch)->player_specials->saved.Rip_mob)
#define GET_RIP_OTHER(ch)      ((ch)->player_specials->saved.Rip_other)
#define GET_RIP_DT(ch)         ((ch)->player_specials->saved.Rip_dt)
#define GET_RIP_PKTHIS(ch)     ((ch)->player_specials->saved.Rip_pk_this)
#define GET_RIP_MOBTHIS(ch)    ((ch)->player_specials->saved.Rip_mob_this)
#define GET_RIP_OTHERTHIS(ch)  ((ch)->player_specials->saved.Rip_other_this)
#define GET_RIP_DTTHIS(ch)     ((ch)->player_specials->saved.Rip_dt_this)
#define GET_EXP_ARENA(ch)      ((ch)->player_specials->saved.Exp_arena)
#define GET_EXP_PK(ch)         ((ch)->player_specials->saved.Exp_pk)
#define GET_EXP_MOB(ch)        ((ch)->player_specials->saved.Exp_mob)
#define GET_EXP_OTHER(ch)      ((ch)->player_specials->saved.Exp_other)
#define GET_EXP_DT(ch)         ((ch)->player_specials->saved.Exp_dt)
#define GET_WIN_ARENA(ch)      ((ch)->player_specials->saved.Win_arena)
#define GET_EXP_PKTHIS(ch)     ((ch)->player_specials->saved.Exp_pk_this)
#define GET_EXP_MOBTHIS(ch)    ((ch)->player_specials->saved.Exp_mob_this)
#define GET_EXP_OTHERTHIS(ch)  ((ch)->player_specials->saved.Exp_other_this)
#define GET_EXP_DTTHIS(ch)     ((ch)->player_specials->saved.Exp_dt_this)
//����� ������ (�) ��������

#define NAME_GOD(ch)  ((ch)->player_specials->saved.NameGod)
#define NAME_ID_GOD(ch)  ((ch)->player_specials->saved.NameIDGod)

#define STRING_LENGTH(ch)  ((ch)->player_specials->saved.stringLength)
#define STRING_WIDTH(ch)  ((ch)->player_specials->saved.stringWidth)
//Polud
#define NOTIFY_EXCH_PRICE(ch)  ((ch)->player_specials->saved.ntfyExchangePrice)

// * I wonder if this definition of GET_REAL_LEVEL should be the definition of GET_LEVEL?  JE
#define GET_REAL_LEVEL(ch) \
   (ch->desc && ch->desc->original ? GET_LEVEL(ch->desc->original) : \
    GET_LEVEL(ch))

#define POSI(val)      ((val < 50) ? ((val > 0) ? val : 1) : 50)

template <typename T>
inline T VPOSI(const T val, const T min, const T max)
{
	return ((val < max) ? ((val > min) ? val : min) : max);
}

// � ����� ����� �� 50, � ����� �� ���
//#define VPOSI_MOB(ch, stat_id, val)	IS_NPC(ch) ? val : VPOSI(val, 1, class_stats_limit[(int)GET_CLASS(ch)][stat_id])

#define GET_CLASS(ch)   ((ch)->get_class())
#define GET_KIN(ch)     ((ch)->player_data.Kin)
#define GET_HEIGHT(ch)  ((ch)->player_data.height)
#define GET_HEIGHT_ADD(ch) ((ch)->add_abils.height_add)
#define GET_REAL_HEIGHT(ch) (GET_HEIGHT(ch) + GET_HEIGHT_ADD(ch))
#define GET_WEIGHT(ch)  ((ch)->player_data.weight)
#define GET_WEIGHT_ADD(ch) ((ch)->add_abils.weight_add)
#define GET_REAL_WEIGHT(ch) (GET_WEIGHT(ch) + GET_WEIGHT_ADD(ch))
#define GET_SEX(ch)  ((ch)->player_data.sex)

#define GET_RELIGION(ch) ((ch)->player_data.Religion)
#define GET_RACE(ch) ((ch)->player_data.Race)
#define GET_PAD(ch,i)    ((ch)->player_data.PNames[i])
#define GET_DRUNK_STATE(ch) ((ch)->player_specials->saved.DrunkState)

#define GET_STR_ADD(ch) ((ch)->get_str_add())
#define GET_REAL_STR(ch) (VPOSI_MOB(ch, 0, ((ch)->get_str() + GET_STR_ADD(ch))))
#define GET_CON_ADD(ch) ((ch)->get_con_add())
#define GET_REAL_CON(ch) (VPOSI_MOB(ch, 2, (ch)->get_con() + GET_CON_ADD(ch)))
#define GET_WIS_ADD(ch) ((ch)->get_wis_add())
#define GET_REAL_WIS(ch) (VPOSI_MOB(ch, 3, ((ch)->get_wis() + GET_WIS_ADD(ch))))
#define GET_INT_ADD(ch) ((ch)->get_int_add())
#define GET_REAL_INT(ch) (VPOSI_MOB(ch, 4, ((ch)->get_int() + GET_INT_ADD(ch))))
#define GET_CHA_ADD(ch) ((ch)->get_cha_add())
#define GET_REAL_CHA(ch) (VPOSI_MOB(ch, 5, ((ch)->get_cha() + GET_CHA_ADD(ch))))
#define GET_SIZE(ch)    ((ch)->real_abils.size)
#define GET_SIZE_ADD(ch)  ((ch)->add_abils.size_add)
#define GET_REAL_SIZE(ch) (VPOSI(GET_SIZE(ch) + GET_SIZE_ADD(ch), 1, 100))
#define GET_POS_SIZE(ch)  (POSI(GET_REAL_SIZE(ch) >> 1))
#define GET_HR(ch)         ((ch)->real_abils.hitroll)
#define GET_HR_ADD(ch)    ((ch)->add_abils.hr_add)
#define GET_REAL_HR(ch)   (VPOSI(GET_HR(ch)+GET_HR_ADD(ch), -50, (IS_MORTIFIER(ch) ? 100 : 50)))
#define GET_DR(ch)         ((ch)->real_abils.damroll)
#define GET_DR_ADD(ch)    ((ch)->add_abils.dr_add)
#define GET_REAL_DR(ch)   (get_real_dr(ch))
#define GET_AC(ch)         ((ch)->real_abils.armor)
#define GET_AC_ADD(ch)    ((ch)->add_abils.ac_add)
#define GET_REAL_AC(ch)      (GET_AC(ch)+GET_AC_ADD(ch))
#define GET_MORALE(ch)       ((ch)->add_abils.morale_add)
#define GET_INITIATIVE(ch)   ((ch)->add_abils.initiative_add)
#define GET_POISON(ch)      ((ch)->add_abils.poison_add)
#define GET_CAST_SUCCESS(ch) ((ch)->add_abils.cast_success)
#define GET_PRAY(ch)         ((ch)->add_abils.pray_add)

#define GET_EXP(ch)           ((ch)->get_exp())
#define GET_HIT(ch)            ((ch)->points.hit)
#define GET_MAX_HIT(ch)       ((ch)->points.max_hit)
#define GET_REAL_MAX_HIT(ch)  (GET_MAX_HIT(ch) + GET_HIT_ADD(ch))
#define GET_MOVE(ch)       ((ch)->points.move)
#define GET_MAX_MOVE(ch)      ((ch)->points.max_move)
#define GET_REAL_MAX_MOVE(ch) (GET_MAX_MOVE(ch) + GET_MOVE_ADD(ch))

#define GET_MANAREG(ch)   ((ch)->add_abils.manareg)
#define GET_HITREG(ch)    ((ch)->add_abils.hitreg)
#define GET_MOVEREG(ch)   ((ch)->add_abils.movereg)
#define GET_ARMOUR(ch)    ((ch)->add_abils.armour)
#define GET_ABSORBE(ch)   ((ch)->add_abils.absorb)
#define GET_AGE_ADD(ch)   ((ch)->add_abils.age_add)
#define GET_HIT_ADD(ch)   ((ch)->add_abils.hit_add)
#define GET_MOVE_ADD(ch)  ((ch)->add_abils.move_add)
#define GET_SAVE(ch,i)    ((ch)->add_abils.apply_saving_throw[i])
#define GET_RESIST(ch,i)  ((ch)->add_abils.apply_resistance_throw[i])
#define GET_AR(ch)        ((ch)->add_abils.aresist)
#define GET_MR(ch)        ((ch)->add_abils.mresist)
#define GET_PR(ch)        ((ch)->add_abils.presist) // added by WorM (�������) ���������� ���.����� � %
#define GET_CASTER(ch)    ((ch)->CasterLevel)
#define GET_DAMAGE(ch)    ((ch)->DamageLevel)
#define GET_LIKES(ch)     ((ch)->mob_specials.LikeWork)

#define GET_REAL_SAVING_STABILITY(ch)	(dex_bonus(GET_REAL_CON(ch)) - GET_SAVE(ch, SAVING_STABILITY)	+ (on_horse(ch) ? 20 : 0))
#define GET_REAL_SAVING_REFLEX(ch)		(dex_bonus(GET_REAL_DEX(ch)) - GET_SAVE(ch, SAVING_REFLEX)		+ (on_horse(ch) ? -20 : 0))
#define GET_REAL_SAVING_CRITICAL(ch)	(dex_bonus(GET_REAL_CON(ch)) - GET_SAVE(ch, SAVING_CRITICAL))
#define GET_REAL_SAVING_WILL(ch)		(dex_bonus(GET_REAL_WIS(ch)) - GET_SAVE(ch, SAVING_WILL))

#define GET_POS(ch)        ((ch)->char_specials.position)
#define GET_IDNUM(ch)     ((ch)->get_idnum())
#define GET_ID(x)         ((x)->id)
#define IS_CARRYING_W(ch) ((ch)->char_specials.carry_weight)
#define IS_CARRYING_N(ch) ((ch)->char_specials.carry_items)

// ������� ������� � ����� ���������� ������
#define GET_ROOM_BASE_POISON(room) ((room)->base_property.poison)
#define GET_ROOM_ADD_POISON(room) ((room)->add_property.poison)
#define GET_ROOM_POISON(room) (GET_ROOM_BASE_POISON(room)+GET_ROOM_ADD_POISON(room))

// ��������� ������� ����� - �������� ������ ��� �����!
#define GET_NDD(ch) ((ch)->mob_specials.damnodice)
#define GET_SDD(ch) ((ch)->mob_specials.damsizedice)

#define ALIG_NEUT 0
#define ALIG_GOOD 1
#define ALIG_EVIL 2
#define ALIG_EVIL_LESS     -300
#define ALIG_GOOD_MORE     300

#define GET_ALIGNMENT(ch)     ((ch)->char_specials.saved.alignment)
#define CALC_ALIGNMENT(ch)    ((GET_ALIGNMENT(ch) <= ALIG_EVIL_LESS) ? ALIG_EVIL : \
                               (GET_ALIGNMENT(ch) >= ALIG_GOOD_MORE) ? ALIG_GOOD : ALIG_NEUT)

#define NAME_LEVEL 5
#define NAME_FINE(ch)          (NAME_GOD(ch)>1000)
#define NAME_BAD(ch)           (NAME_GOD(ch)<1000 && NAME_GOD(ch))

#define MAX_EXP_PERCENT   80
#define MAX_EXP_RMRT_PERCENT(ch) (MAX_EXP_PERCENT+ch->get_remort()*5)

#define GET_COND(ch, i)    CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->saved.conditions[(i)]))
#define GET_LOADROOM(ch)   CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->saved.load_room))
#define GET_WIMP_LEV(ch)   CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->saved.wimp_level))
#define GET_BAD_PWS(ch)    CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->saved.bad_pws))
#define POOFIN(ch)              CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->poofin))
#define POOFOUT(ch)     CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->poofout))
#define RENTABLE(ch)    CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->may_rent))
#define AGRESSOR(ch)    CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->agressor))
#define AGRO(ch)    CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->agro_time))

#define EXCHANGE_FILTER(ch)     CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->Exchange_filter))

#define GET_ALIASES(ch)        CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->aliases))
#define GET_RSKILL(ch)          CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->rskill))
#define GET_PORTALS(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->portals))
#define GET_LOGS(ch)            CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->logs))

// Punishments structs
#define MUTE_REASON(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pmute.reason))
#define DUMB_REASON(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pdumb.reason))
#define HELL_REASON(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->phell.reason))
#define FREEZE_REASON(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pfreeze.reason))
#define GCURSE_REASON(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pgcurse.reason))
#define NAME_REASON(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pname.reason))
#define UNREG_REASON(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->punreg.reason))

#define GET_MUTE_LEV(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pmute.level))
#define GET_DUMB_LEV(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pdumb.level))
#define GET_HELL_LEV(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->phell.level))
#define GET_FREEZE_LEV(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pfreeze.level))
#define GET_GCURSE_LEV(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pgcurse.level))
#define GET_NAME_LEV(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pname.level))
#define GET_UNREG_LEV(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->punreg.level))

#define MUTE_GODID(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pmute.godid))
#define DUMB_GODID(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pdumb.godid))
#define HELL_GODID(ch)         CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->phell.godid))
#define FREEZE_GODID(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pfreeze.godid))
#define GCURSE_GODID(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pgcurse.godid))
#define NAME_GODID(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pname.godid))
#define UNREG_GODID(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->punreg.godid))

#define GCURSE_DURATION(ch)  CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pgcurse.duration))
#define MUTE_DURATION(ch)  CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pmute.duration))
#define DUMB_DURATION(ch)  CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pdumb.duration))
#define FREEZE_DURATION(ch)  CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pfreeze.duration))
#define HELL_DURATION(ch)  CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->phell.duration))
#define NAME_DURATION(ch)  CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->pname.duration))
#define UNREG_DURATION(ch)  CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->punreg.duration))

#define KARMA(ch) CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->Karma))
#define LOGON_LIST(ch) CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->logons))

#define CLAN(ch)              CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->clan))
#define CLAN_MEMBER(ch)       CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->clan_member))
#define GET_CLAN_STATUS(ch)   CHECK_PLAYER_SPECIAL((ch), ((ch)->player_specials->clanStatus))

#define GET_SPELL_TYPE(ch, i) ((ch)->real_abils.SplKnw[i])
#define GET_SPELL_MEM(ch, i)  ((ch)->real_abils.SplMem[i])
#define SET_SPELL(ch, i, pct) ((ch)->real_abils.SplMem[i] = pct)
#define SET_FEAT(ch, feat) ((ch)->real_abils.Feats.set(feat))
#define UNSET_FEAT(ch, feat) ((ch)->real_abils.Feats.reset(feat))
#define HAVE_FEAT(ch, feat) ((ch)->real_abils.Feats.test(feat))
#define	NUM_LEV_FEAT(ch) ((int) 1+GET_LEVEL(ch)*(5+GET_REMORT(ch)/feat_slot_for_remort[(int) GET_CLASS(ch)])/28)
#define FEAT_SLOT(ch, feat) (feat_info[feat].slot[(int) GET_CLASS(ch)][(int) GET_KIN(ch)])


// Min cast level getting
#define MIN_CAST_LEV(sp, ch) (MMAX(0,MOD_CAST_LEV(sp,ch)))
#define MOD_CAST_LEV(sp, ch) (BASE_CAST_LEV(sp, ch) - (MMAX(GET_REMORT(ch) - MIN_CAST_REM(sp,ch),0) / 3))
#define BASE_CAST_LEV(sp, ch) ((sp).min_level[(int) GET_CLASS (ch)][(int) GET_KIN (ch)])

#define MIN_CAST_REM(sp, ch) ((sp).min_remort[(int) GET_CLASS (ch)][(int) GET_KIN (ch)])

#define GET_EQ(ch, i)      ((ch)->equipment[i])

#define GET_MOB_SPEC(ch)   (IS_MOB(ch) ? mob_index[(ch)->nr].func : NULL)
#define GET_MOB_RNUM(mob)  ((mob)->nr)
#define GET_MOB_VNUM(mob)  (IS_MOB(mob) ? \
             mob_index[GET_MOB_RNUM(mob)].vnum : -1)

#define GET_DEFAULT_POS(ch)   ((ch)->mob_specials.default_pos)
#define MEMORY(ch)          ((ch)->mob_specials.memory)
#define GET_DEST(ch)        (((ch)->mob_specials.dest_count ? \
                              (ch)->mob_specials.dest[(ch)->mob_specials.dest_pos] : \
                              NOWHERE))
#define GET_ACTIVITY(ch)    ((ch)->mob_specials.activity)
#define GET_GOLD_NoDs(ch)   ((ch)->mob_specials.GoldNoDs)
#define GET_GOLD_SiDs(ch)   ((ch)->mob_specials.GoldSiDs)
#define GET_HORSESTATE(ch)  ((ch)->mob_specials.HorseState)
#define GET_LASTROOM(ch)    ((ch)->mob_specials.LastRoom)

#define CAN_SEE_IN_DARK(ch) \
   (AFF_FLAGGED(ch, EAffectFlag::AFF_INFRAVISION) || (!IS_NPC(ch) && PRF_FLAGGED(ch, PRF_HOLYLIGHT)))

#define IS_GOOD(ch)          (GET_ALIGNMENT(ch) >= ALIG_GOOD_MORE)
#define IS_EVIL(ch)          (GET_ALIGNMENT(ch) <= ALIG_EVIL_LESS)
#define IS_NEUTRAL(ch)        (!IS_GOOD(ch) && !IS_EVIL(ch))
/*
#define SAME_ALIGN(ch,vict)  ((IS_GOOD(ch) && IS_GOOD(vict)) ||\
                              (IS_EVIL(ch) && IS_EVIL(vict)) ||\
               (IS_NEUTRAL(ch) && IS_NEUTRAL(vict)))
*/
#define ALIGN_DELTA  10
#define SAME_ALIGN(ch,vict)  (GET_ALIGNMENT(ch)>GET_ALIGNMENT(vict)?\
                              (GET_ALIGNMENT(ch)-GET_ALIGNMENT(vict))<=ALIGN_DELTA:\
                              (GET_ALIGNMENT(vict)-GET_ALIGNMENT(ch))<=ALIGN_DELTA\
                             )
#define GET_CH_SUF_1(ch) (IS_NOSEXY(ch) ? "�" :\
                          IS_MALE(ch) ? ""  :\
                          IS_FEMALE(ch) ? "�" : "�")
#define GET_CH_SUF_2(ch) (IS_NOSEXY(ch) ? "���" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "���" : "���")
#define GET_CH_SUF_3(ch) (IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "��")
#define GET_CH_SUF_4(ch) (IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? ""  :\
                          IS_FEMALE(ch) ? "��" : "��")
#define GET_CH_SUF_5(ch) (IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "��")
#define GET_CH_SUF_6(ch) (IS_NOSEXY(ch) ? "�" :\
                          IS_MALE(ch) ? ""  :\
                          IS_FEMALE(ch) ? "�" : "�")
#define GET_CH_SUF_7(ch) (IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "���")
#define GET_CH_SUF_8(ch) (IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "��")

#define GET_CH_VIS_SUF_1(ch,och) (!CAN_SEE(och,ch) ? "" :\
                          IS_NOSEXY(ch) ? "�" :\
                          IS_MALE(ch) ? ""  :\
                          IS_FEMALE(ch) ? "�" : "�")
#define GET_CH_VIS_SUF_2(ch,och) (!CAN_SEE(och,ch) ? "��" :\
                          IS_NOSEXY(ch) ? "���" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "���" : "���")
#define GET_CH_VIS_SUF_3(ch,och) (!CAN_SEE(och,ch) ? "��" :\
                          IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "��")
#define GET_CH_VIS_SUF_4(ch,och) (!CAN_SEE(och,ch) ? "" :\
                          IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? ""  :\
                          IS_FEMALE(ch) ? "��" : "��")
#define GET_CH_VIS_SUF_5(ch,och) (!CAN_SEE(och,ch) ? "��" :\
                          IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "��")
#define GET_CH_VIS_SUF_6(ch,och) (!CAN_SEE(och,ch) ? "" :\
                          IS_NOSEXY(ch) ? "�" :\
                          IS_MALE(ch) ? ""  :\
                          IS_FEMALE(ch) ? "�" : "�")
#define GET_CH_VIS_SUF_7(ch,och) (!CAN_SEE(och,ch) ? "��" :\
                          IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "���")
#define GET_CH_VIS_SUF_8(ch,och) (!CAN_SEE(och,ch) ? "��" :\
                          IS_NOSEXY(ch) ? "��" :\
                          IS_MALE(ch) ? "��"  :\
                          IS_FEMALE(ch) ? "��" : "��")


#define GET_OBJ_SEX(obj) ((obj)->get_sex())
#define IS_OBJ_NOSEXY(obj)    (GET_OBJ_SEX(obj) == ESex::SEX_NEUTRAL)
#define IS_OBJ_MALE(obj)   (GET_OBJ_SEX(obj) == ESex::SEX_MALE)
#define IS_OBJ_FEMALE(obj)    (GET_OBJ_SEX(obj) == ESex::SEX_FEMALE)

#define GET_OBJ_MIW(obj) ((obj)->get_max_in_world())

#define GET_OBJ_SUF_1(obj) (IS_OBJ_NOSEXY(obj) ? "�" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "�" : "�")
#define GET_OBJ_SUF_2(obj) (IS_OBJ_NOSEXY(obj) ? "���" :\
                            IS_OBJ_MALE(obj) ? "��"  :\
                            IS_OBJ_FEMALE(obj) ? "���" : "���")
#define GET_OBJ_SUF_3(obj) (IS_OBJ_NOSEXY(obj) ? "��" :\
                            IS_OBJ_MALE(obj) ? "��"  :\
                            IS_OBJ_FEMALE(obj) ? "��" : "��")
#define GET_OBJ_SUF_4(obj) (IS_OBJ_NOSEXY(obj) ? "��" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "��" : "��")
#define GET_OBJ_SUF_5(obj) (IS_OBJ_NOSEXY(obj) ? "��" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "��" : "��")
#define GET_OBJ_SUF_6(obj) (IS_OBJ_NOSEXY(obj) ? "�" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "�" : "�")
#define GET_OBJ_SUF_7(obj) (IS_OBJ_NOSEXY(obj) ? "�" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "�" : "�")
#define GET_OBJ_SUF_8(ch) (IS_OBJ_NOSEXY(obj) ? "��" :\
                          IS_OBJ_MALE(obj) ? "��"  :\
                          IS_OBJ_FEMALE(obj) ? "��" : "��")


#define GET_OBJ_VIS_SUF_1(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "�" :\
                            IS_OBJ_NOSEXY(obj) ? "�" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "�" : "�")
#define GET_OBJ_VIS_SUF_2(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "���" :\
                            IS_OBJ_NOSEXY(obj) ? "���" :\
                            IS_OBJ_MALE(obj) ? "��"  :\
                            IS_OBJ_FEMALE(obj) ? "���" : "���")
#define GET_OBJ_VIS_SUF_3(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "��" :\
                            IS_OBJ_NOSEXY(obj) ? "��" :\
                            IS_OBJ_MALE(obj) ? "��"  :\
                            IS_OBJ_FEMALE(obj) ? "��" : "��")
#define GET_OBJ_VIS_SUF_4(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "��" :\
                            IS_OBJ_NOSEXY(obj) ? "��" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "��" : "��")
#define GET_OBJ_VIS_SUF_5(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "��" :\
                            IS_OBJ_NOSEXY(obj) ? "��" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "��" : "��")
#define GET_OBJ_VIS_SUF_6(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "�" :\
                            IS_OBJ_NOSEXY(obj) ? "�" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "�" : "�")
#define GET_OBJ_VIS_SUF_7(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "�" :\
                            IS_OBJ_NOSEXY(obj) ? "�" :\
                            IS_OBJ_MALE(obj) ? ""  :\
                            IS_OBJ_FEMALE(obj) ? "�" : "�")
#define GET_OBJ_VIS_SUF_8(obj,ch) (!CAN_SEE_OBJ(ch,obj) ? "��" :\
                          IS_OBJ_NOSEXY(obj) ? "��" :\
                          IS_OBJ_MALE(obj) ? "��"  :\
                          IS_OBJ_FEMALE(obj) ? "��" : "��")

#define GET_CH_EXSUF_1(ch) (IS_NOSEXY(ch) ? "��" :\
                            IS_MALE(ch) ? "��"  :\
                            IS_FEMALE(ch) ? "��" : "���")
#define GET_CH_POLY_1(ch) (IS_POLY(ch) ? "��" : "")

#define GET_OBJ_POLY_1(ch, obj) ((GET_OBJ_SEX(obj) == ESex::SEX_POLY) ? "��" : "��")
#define GET_OBJ_VIS_POLY_1(ch, obj) (!CAN_SEE_OBJ(ch,obj) ? "��" : (GET_OBJ_SEX(obj) == ESex::SEX_POLY) ? "��" : "��")

#define PUNCTUAL_WAIT_STATE(ch, cycle) do { GET_PUNCTUAL_WAIT_STATE(ch) = (cycle); } while(0)
#define CHECK_WAIT(ch)        ((ch)->get_wait() > 0)
#define GET_WAIT(ch)          (ch)->get_wait()
#define GET_PUNCTUAL_WAIT(ch)          GET_PUNCTUAL_WAIT_STATE(ch)
#define GET_MOB_SILENCE(ch)  (AFF_FLAGGED((ch),AFF_SILENCE) ? 1 : 0)
// New, preferred macro
#define GET_PUNCTUAL_WAIT_STATE(ch)    ((ch)->punctual_wait)


// descriptor-based utils ***********************************************

// Hrm, not many.  We should make more. -gg 3/4/99
#define STATE(d)  ((d)->connected)


// object utils *********************************************************
#define GET_OBJ_UID(obj)	((obj)->get_uid())

#define GET_OBJ_ALIAS(obj)      ((obj)->get_aliases())
#define GET_OBJ_PNAME(obj,pad)  ((obj)->get_PName(pad))
#define GET_OBJ_DESC(obj)       ((obj)->get_description())
#define GET_OBJ_SPELL(obj)      ((obj)->get_spell())
#define GET_OBJ_LEVEL(obj)      ((obj)->get_level())
#define GET_OBJ_AFFECTS(obj)    ((obj)->get_affect_flags())
#define GET_OBJ_ANTI(obj)       ((obj)->get_anti_flags())
#define GET_OBJ_NO(obj)         ((obj)->get_no_flags())
#define GET_OBJ_ACT(obj)        ((obj)->get_action_description())
#define GET_OBJ_POS(obj)        ((obj)->get_worn_on())
#define GET_OBJ_TYPE(obj)       ((obj)->get_type())
#define GET_OBJ_COST(obj)       ((obj)->get_cost())
#define GET_OBJ_RENT(obj)       ((obj)->get_rent_off())
#define GET_OBJ_RENTEQ(obj)     ((obj)->get_rent_on())
#define GET_OBJ_EXTRA(obj)  ((obj)->get_extra_flags())
#define GET_OBJ_WEAR(obj)  ((obj)->get_wear_flags())
#define GET_OBJ_OWNER(obj)      ((obj)->get_owner())
#define GET_OBJ_MAKER(obj)      ((obj)->get_crafter_uid())
#define GET_OBJ_PARENT(obj)      ((obj)->get_parent())
#define GET_OBJ_RENAME(obj)      ((obj)->get_is_rename())
#define GET_OBJ_CRAFTIMER(obj)      ((obj)->get_craft_timer())
#define GET_OBJ_VAL(obj, val) ((obj)->get_val((val)))
#define GET_OBJ_WEIGHT(obj)   ((obj)->get_weight())
#define GET_OBJ_DESTROY(obj) ((obj)->get_destroyer())
#define GET_OBJ_SKILL(obj) ((obj)->get_skill())
#define GET_OBJ_CUR(obj)    ((obj)->get_current_durability())
#define GET_OBJ_MAX(obj)    ((obj)->get_maximum_durability())
#define GET_OBJ_MATER(obj)  ((obj)->get_material())
#define GET_OBJ_ZONE(obj)   ((obj)->get_zone())
#define GET_OBJ_RNUM(obj)  ((obj)->get_rnum())
#define OBJ_GET_LASTROOM(obj) ((obj)->get_room_was_in())
#define OBJ_WHERE(obj) ((obj)->get_worn_by() ? IN_ROOM(obj->get_worn_by()) : \
                        (obj)->get_carried_by() ? IN_ROOM(obj->get_carried_by()) : (obj)->get_in_room())
#define IS_OBJ_ANTI(obj,stat) ((obj)->get_anti_flag(stat))
#define IS_OBJ_NO(obj,stat) ((obj)->get_no_flag(stat))
#define IS_OBJ_AFF(obj,stat) (obj->get_affect(stat))

#define IS_CORPSE(obj)     (GET_OBJ_TYPE(obj) == OBJ_DATA::ITEM_CONTAINER && \
               GET_OBJ_VAL((obj), 3) == 1)
#define IS_MOB_CORPSE(obj) (IS_CORPSE(obj) &&  GET_OBJ_VAL((obj), 2) != -1)

// compound utilities and other macros *********************************

/*
 * Used to compute CircleMUD version. To see if the code running is newer
 * than 3.0pl13, you would use: #if _CIRCLEMUD > CIRCLEMUD_VERSION(3,0,13)
 */
#define CIRCLEMUD_VERSION(major, minor, patchlevel) \
   (((major) << 16) + ((minor) << 8) + (patchlevel))

#define HSHR(ch) (ESex::SEX_NEUTRAL != GET_SEX(ch) ? (IS_MALE(ch) ? "���": (IS_FEMALE(ch) ? "��" : "��")) :"���")
#define HSSH(ch) (ESex::SEX_NEUTRAL != GET_SEX(ch) ? (IS_MALE(ch) ? "��": (IS_FEMALE(ch) ? "���" : "���")) :"���")
#define HMHR(ch) (ESex::SEX_NEUTRAL != GET_SEX(ch) ? (IS_MALE(ch) ? "���": (IS_FEMALE(ch) ? "��" : "��")) :"���")
#define HYOU(ch) (ESex::SEX_NEUTRAL != GET_SEX(ch) ? (IS_MALE(ch) ? "���": (IS_FEMALE(ch) ? "����" : (IS_NOSEXY(ch) ? "����": "����"))) :"���")

#define OSHR(ch) (ESex::SEX_NEUTRAL != GET_OBJ_SEX(ch) ? (GET_OBJ_SEX(ch) == ESex::SEX_MALE ? "���": (GET_OBJ_SEX(ch) == ESex::SEX_FEMALE ? "��" : "��")) :"���")
#define OSSH(ch) (ESex::SEX_NEUTRAL != GET_OBJ_SEX(ch) ? (GET_OBJ_SEX(ch) == ESex::SEX_MALE ? "��": (GET_OBJ_SEX(ch) == ESex::SEX_FEMALE ? "���" : "���")) :"���")
#define OMHR(ch) (ESex::SEX_NEUTRAL != GET_OBJ_SEX(ch) ? (GET_OBJ_SEX(ch) == ESex::SEX_MALE ? "���": (GET_OBJ_SEX(ch) == ESex::SEX_FEMALE ? "��" : "��")) :"���")
#define OYOU(ch) (ESex::SEX_NEUTRAL != GET_OBJ_SEX(ch) ? (GET_OBJ_SEX(ch) == ESex::SEX_MALE ? "���": (GET_OBJ_SEX(ch) == ESex::SEX_FEMALE ? "����" : "����")) :"����")

#define HERE(ch)  ((IS_NPC(ch) || (ch)->desc || RENTABLE(ch)))

// Can subject see character "obj" without light
#define MORT_CAN_SEE_CHAR(sub, obj) (HERE(obj) && \
                                     INVIS_OK(sub,obj) \
                )

#define IMM_CAN_SEE_CHAR(sub, obj) \
        (MORT_CAN_SEE_CHAR(sub, obj) || (!IS_NPC(sub) && PRF_FLAGGED(sub, PRF_HOLYLIGHT)))

#define CAN_SEE_CHAR(sub, obj) (IS_CODER(sub) || SELF(sub, obj) || \
        ((GET_REAL_LEVEL(sub) >= (IS_NPC(obj) ? 0 : GET_INVIS_LEV(obj))) && \
         IMM_CAN_SEE_CHAR(sub, obj)))
// End of CAN_SEE

// Is anyone carrying this object and if so, are they visible?
#define CAN_SEE_OBJ_CARRIER(sub, obj) \
  ((!obj->carried_by || CAN_SEE(sub, obj->carried_by)) && \
   (!obj->worn_by    || CAN_SEE(sub, obj->worn_by)))

#define GET_PAD_PERS(pad) ((pad) == 5 ? "���-��" :\
                           (pad) == 4 ? "���-��" :\
                           (pad) == 3 ? "����-��" :\
                           (pad) == 2 ? "����-��" :\
                           (pad) == 1 ? "����-��" : "���-��")

#define PERS(ch,vict,pad) (CAN_SEE(vict, ch) ? GET_PAD(ch,pad) : GET_PAD_PERS(pad))
//��� �����
#define APERS(ch,vict,pad,arena) ((arena) || CAN_SEE(vict, ch) ? GET_PAD(ch,pad) : GET_PAD_PERS(pad))

//��� �����
#define AOBJS(obj,vict,arena) ((arena) || CAN_SEE_OBJ((vict), (obj)) ? \
                      (obj)->get_short_description().c_str() : "���-��")

#define GET_PAD_OBJ(pad)  ((pad) == 5 ? "���-��" :\
                           (pad) == 4 ? "���-��" :\
                           (pad) == 3 ? "���-��" :\
                           (pad) == 2 ? "����-��" :\
                           (pad) == 1 ? "����-��" : "���-��")

//��� �����
#define AOBJN(obj,vict,pad,arena) ((arena) || CAN_SEE_OBJ((vict), (obj)) ? \
                           (!(obj)->get_PName(pad).empty()) ? (obj)->get_PName(pad).c_str() : (obj)->get_short_description().c_str() \
                           : GET_PAD_OBJ(pad))

#define EXITDATA(room,door) ((room >= 0 && room <= top_of_world) ? \
                             world[room]->dir_option[door] : NULL)

#define EXIT(ch, door)  (world[(ch)->in_room]->dir_option[door])

#define CAN_GO(ch, door) (ch?((EXIT(ch,door) && \
          (EXIT(ch,door)->to_room != NOWHERE) && \
          !IS_SET(EXIT(ch, door)->exit_info, EX_CLOSED))):0)


#define CLASS_ABBR(ch) (IS_NPC(ch) ? "--" : class_abbrevs[(int)GET_CLASS(ch)])

#define KIN_ABBR(ch) (IS_NPC(ch) ? "--" : kin_abbrevs[(int)GET_KIN(ch)])

#define IS_MAGIC_USER(ch)  (!IS_NPC(ch) && \
            (IS_BITS(MASK_MAGES, (int) GET_CLASS(ch))))
#define IS_CLERIC(ch)      (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_CLERIC))
#define IS_THIEF(ch)    (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_THIEF))
#define IS_ASSASINE(ch)    (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_ASSASINE))
#define IS_WARRIOR(ch)     (!IS_NPC(ch) && \
            (GET_CLASS(ch) == CLASS_WARRIOR))
#define IS_PALADINE(ch)    (!IS_NPC(ch) && \
            (GET_CLASS(ch) == CLASS_PALADINE))
#define IS_RANGER(ch)      (!IS_NPC(ch) && \
            (GET_CLASS(ch) == CLASS_RANGER))
#define IS_GUARD(ch)    (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_GUARD))
#define IS_SMITH(ch)    (!IS_NPC(ch) && \
            (GET_CLASS(ch) == CLASS_SMITH))
#define IS_MERCHANT(ch)    (!IS_NPC(ch) && \
            (GET_CLASS(ch) == CLASS_MERCHANT))
#define IS_DRUID(ch)    (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_DRUID))
#define IS_BATTLEMAGE(ch)  (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_BATTLEMAGE))
#define IS_CHARMMAGE(ch)   (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_CHARMMAGE))
#define IS_DEFENDERMAGE(ch)   (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_DEFENDERMAGE))
#define IS_NECROMANCER(ch) (!IS_NPC(ch) && \
            ((int) GET_CLASS(ch) == CLASS_NECROMANCER))

#define IS_UNDEAD(ch) (IS_NPC(ch) && \
	(MOB_FLAGGED(ch, MOB_RESURRECTED) || (GET_RACE(ch) == NPC_RACE_ZOMBIE)))

#define LIKE_ROOM(ch) ((IS_CLERIC(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_CLERIC)) || \
                       (IS_MAGIC_USER(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_MAGE)) || \
                       (IS_WARRIOR(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_WARRIOR)) || \
                       (IS_THIEF(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_THIEF)) || \
                       (IS_ASSASINE(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_ASSASINE)) || \
                       (IS_GUARD(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_GUARD)) || \
                       (IS_PALADINE(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_PALADINE)) || \
                       (IS_RANGER(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_RANGER)) || \
                       (IS_SMITH(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_SMITH)) || \
                       (IS_MERCHANT(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_MERCHANT)) || \
                       (IS_DRUID(ch) && ROOM_FLAGGED((ch)->in_room, ROOM_DRUID)))

#define OUTSIDE(ch) (!ROOM_FLAGGED((ch)->in_room, ROOM_INDOORS))

int on_horse(CHAR_DATA * ch);
int has_horse(CHAR_DATA * ch, int same_room);
CHAR_DATA *get_horse(CHAR_DATA * ch);
void horse_drop(CHAR_DATA * ch);
void make_horse(CHAR_DATA * horse, CHAR_DATA * ch);
void check_horse(CHAR_DATA * ch);

bool same_group(CHAR_DATA * ch, CHAR_DATA * tch);

int is_post(room_rnum room);
bool is_rent(room_rnum room);

int pc_duration(CHAR_DATA * ch, int cnst, int level, int level_divisor, int min, int max);

// Modifier functions
int day_spell_modifier(CHAR_DATA * ch, int spellnum, int type, int value);
int weather_spell_modifier(CHAR_DATA * ch, int spellnum, int type, int value);
int complex_spell_modifier(CHAR_DATA * ch, int spellnum, int type, int value);

int day_skill_modifier(CHAR_DATA * ch, int skillnum, int type, int value);
int weather_skill_modifier(CHAR_DATA * ch, int skillnum, int type, int value);
int complex_skill_modifier(CHAR_DATA * ch, int skillnum, int type, int value);
void can_carry_obj(CHAR_DATA * ch, OBJ_DATA * obj);
bool CAN_CARRY_OBJ(const CHAR_DATA *ch, const OBJ_DATA *obj);
bool ignores(CHAR_DATA *, CHAR_DATA *, unsigned int);

// PADS for something ***************************************************
const char * desc_count(long how_many, int of_what);
#define WHAT_DAY	0
#define WHAT_HOUR	1
#define WHAT_YEAR	2
#define WHAT_POINT	3
#define WHAT_MINa	4
#define WHAT_MINu	5
#define WHAT_MONEYa	6
#define WHAT_MONEYu	7
#define WHAT_THINGa	8
#define WHAT_THINGu	9
#define WHAT_LEVEL	10
#define WHAT_MOVEa	11
#define WHAT_MOVEu	12
#define WHAT_ONEa	13
#define WHAT_ONEu	14
#define WHAT_SEC	15
#define WHAT_DEGREE	16
#define WHAT_ROW	17
#define WHAT_OBJECT	18
#define WHAT_OBJu	19
#define WHAT_REMORT	20
#define WHAT_WEEK	21
#define WHAT_MONTH	22
#define WHAT_WEEKu	23
#define WHAT_GLORY	24
#define WHAT_GLORYu	25
#define WHAT_PEOPLE	26
#define WHAT_STR	27
#define WHAT_GULP	28
#define WHAT_TORC	29
#define WHAT_TGOLD		30
#define WHAT_TSILVER	31
#define WHAT_TBRONZE	32
#define WHAT_TORCu		33
#define WHAT_TGOLDu		34
#define WHAT_TSILVERu	35
#define WHAT_TBRONZEu	36
#define WHAT_ICEu		37

#undef AW_HIDE // ����������� � winuser.h
// some awaking cases
#define AW_HIDE       (1 << 0)
#define AW_INVIS      (1 << 1)
#define AW_CAMOUFLAGE (1 << 2)
#define AW_SNEAK      (1 << 3)

#define ACHECK_AFFECTS (1 << 0)
#define ACHECK_LIGHT   (1 << 1)
#define ACHECK_HUMMING (1 << 2)
#define ACHECK_GLOWING (1 << 3)
#define ACHECK_WEIGHT  (1 << 4)

int check_awake(CHAR_DATA * ch, int what);
int awake_hide(CHAR_DATA * ch);
int awake_invis(CHAR_DATA * ch);
int awake_camouflage(CHAR_DATA * ch);
int awake_sneak(CHAR_DATA * ch);
int awaking(CHAR_DATA * ch, int mode);
std::string time_format(int timer, int flag = 0);

size_t count_colors(const char * str, size_t len = 0);
char* colored_name(const char * str, size_t len, const bool left_align = false);
size_t strlen_no_colors(const char *str);

// OS compatibility *****************************************************


// there could be some strange OS which doesn't have NULL...
#ifndef NULL
#define NULL (void *)0
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE  (!FALSE)
#endif

// defines for fseek
#ifndef SEEK_SET
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2
#endif

#define SENDOK(ch)   (((ch)->desc || SCRIPT_CHECK((ch), MTRIG_ACT)) && \
               (to_sleeping || AWAKE(ch)) && \
                     !PLR_FLAGGED((ch), PLR_WRITING))



inline bool a_isspace(unsigned char c)
{
	return (strchr(" \f\n\r\t\v", c) != NULL);
}

// ������ �� ��� �� ��������� ������� ������������ � ����, �� ����� ����� (���������� � ���� AL'��)
inline bool a_isascii(unsigned char c)
{
	return c >= 32;
}

inline bool a_isprint(unsigned char c)
{
	return c >= 32;
}

inline bool a_islower(unsigned char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 192 && c <= 223) || c == 163;
}

inline bool a_isupper(unsigned char c)
{
	return (c >= 'A' && c <= 'Z') || c >= 224 || c == 179;
}

inline bool a_isdigit(unsigned char c)
{
	return c >= '0' && c <= '9';
}

inline bool a_isalpha(unsigned char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 192 || c == 163 || c == 179;
}

extern const bool a_isalnum_table[];
inline bool a_isalnum(unsigned char c)
{
	return a_isalnum_table[c];
}

inline bool a_isxdigit(unsigned char c)
{
	return (c >= '0' && c <= '9')
		   || (c >= 'a' && c <= 'f')
		   || (c >= 'A' && c <= 'F');
}

inline char a_ucc(unsigned char c)
{
	if (c >= 'a' && c <= 'z') return c - 'a' + 'A';
	if (c >= 192 && c <= 223) return c + 32;
	if (c == 163) return c + 16;
	return c;
}

extern const char a_lcc_table[];
inline char a_lcc(unsigned char c)
{
	return a_lcc_table[c];
}

enum separator_mode
{
	A_ISSPACE,
	A_ISASCII,
	A_ISPRINT,
	A_ISLOWER,
	A_ISUPPER,
	A_ISDIGIT,
	A_ISALPHA,
	A_ISALNUM,
	A_ISXDIGIT
};

class pred_separator
{
	bool (*pred)(unsigned char);
	bool l_not;
public:
	explicit
	pred_separator(separator_mode __mode, bool __l_not = false) : l_not(__l_not)
	{
		switch (__mode)
		{
		case A_ISSPACE:
			pred = a_isspace;
			break;
		case A_ISASCII:
			pred = a_isascii;
			break;
		case A_ISPRINT:
			pred = a_isprint;
			break;
		case A_ISLOWER:
			pred = a_islower;
			break;
		case A_ISUPPER:
			pred = a_isupper;
			break;
		case A_ISDIGIT:
			pred = a_isdigit;
			break;
		case A_ISALPHA:
			pred = a_isalpha;
			break;
		case A_ISALNUM:
			pred = a_isalnum;
			break;
		case A_ISXDIGIT:
			pred = a_isxdigit;
			break;
		}
	}

	explicit
	pred_separator() : pred(a_isspace), l_not(false) {}

	void reset() {}

	bool operator()(std::string::const_iterator& next, std::string::const_iterator end, std::string& tok)
	{
		tok = std::string();

		if (l_not)
			for (; next != end && !pred(*next); ++next) {}
		else
			for (; next != end && pred(*next); ++next) {}

		if (next == end)
			return false;

		if (l_not)
			for (; next != end && pred(*next); ++next)
				tok += *next;
		else
			for (; next != end && !pred(*next); ++next)
				tok += *next;

		return true;
	}
};

template<class T> void skip_spaces(T string)
{
	for (; **string && a_isspace(**string); (*string)++) ;
}

namespace MoneyDropStat
{

void add(int zone_vnum, long money);
void print(CHAR_DATA *ch);
void print_log();

} // MoneyDropStat

namespace ZoneExpStat
{

void add(int zone_vnum, long exp);
void print_gain(CHAR_DATA *ch);
void print_log();

} // ZoneExpStat

std::string thousands_sep(long long n);

enum { STR_TO_HIT, STR_TO_DAM, STR_CARRY_W, STR_WIELD_W, STR_HOLD_W, STR_BOTH_W, STR_SHIELD_W };
enum { WIS_MAX_LEARN_L20, WIS_SPELL_SUCCESS, WIS_MAX_SKILLS, WIS_FAILS };

int str_bonus(int str, int type);
int dex_bonus(int dex);
int dex_ac_bonus(int dex);
int calc_str_req(int weight, int type);
void message_str_need(CHAR_DATA *ch, OBJ_DATA *obj, int type);
int wis_bonus(int stat, int type);
int CAN_CARRY_N(const CHAR_DATA* ch);

#define CAN_CARRY_W(ch) ((str_bonus(GET_REAL_STR(ch), STR_CARRY_W) * (HAVE_FEAT(ch, PORTER_FEAT) ? 110 : 100))/100)

#define OK_BOTH(ch,obj)  (GET_OBJ_WEIGHT(obj) <= \
                          str_bonus(GET_REAL_STR(ch), STR_WIELD_W) + str_bonus(GET_REAL_STR(ch), STR_HOLD_W))

#define OK_WIELD(ch,obj) (GET_OBJ_WEIGHT(obj) <= \
                          str_bonus(GET_REAL_STR(ch), STR_WIELD_W))

#define OK_HELD(ch,obj)  (GET_OBJ_WEIGHT(obj) <= \
                          str_bonus(GET_REAL_STR(ch), STR_HOLD_W))

#define OK_SHIELD(ch,obj)  (GET_OBJ_WEIGHT(obj) <= \
                          (2 * str_bonus(GET_REAL_STR(ch), STR_HOLD_W)))

/// ������ sprintbitwd � �����������
/// \param bits - bitset|boost::dynamic_bitset
/// \param names - vector|array<string|const char*> ������ �������� �����
/// div - ����������� ����� ������ ��� ����������
/// str - ������, ���� �������� ����� ����� (����������� � �����)
/// print_num - ������ ����� ���� ����� � ��� ������
/// 	(��� ���, ���� ����� ���������� � 1), �� ������� = false
template <class T, class N>
void print_bitset(const N& bits, const T& names,
	const char* div, std::string& str, bool print_num = false)
{
	static char tmp_buf[10];
	bool first = true;

	for (unsigned i = 0; i < bits.size(); ++i)
	{
		if (bits.test(i) == true)
		{
			if (!first)
			{
				str += div;
			}
			else
			{
				first = false;
			}

			if (print_num)
			{
				snprintf(tmp_buf, sizeof(tmp_buf), "%d:", i + 1);
				str += tmp_buf;
			}

			if (i < names.size())
			{
				str += names[i];
			}
			else
			{
				str += "UNDEF";
			}
		}
	}
}

const char *print_obj_state(int tm_pct);

bool no_bad_affects(OBJ_DATA *obj);

struct exchange_item_data;
// ��� ����� ������ � ��������� � ����-������ � ������
struct ParseFilter
{
	enum { CLAN, EXCHANGE };

	ParseFilter(int type) : type(-1), state(-1), wear(EWearFlag::ITEM_WEAR_UNDEFINED), wear_message(-1),
		weap_class(-1), weap_message(-1), cost(-1), cost_sign('\0'),rent(-1), rent_sign('\0'),
		new_timesign('\0'), new_timedown(time(0)), new_timeup(time(0)),
		filter_type(type) {};

	bool init_type(const char *str);
	bool init_state(const char *str);
	bool init_wear(const char *str);
	bool init_cost(const char *str);
	bool init_rent(const char *str);
	bool init_weap_class(const char *str);
	bool init_affect(char *str, size_t str_len);
	bool init_realtime(const char *str);
	size_t affects_cnt() const;
	bool check(OBJ_DATA *obj, CHAR_DATA *ch);
	bool check(exchange_item_data *exch_obj);
	std::string print() const;

	std::string name;      // ��� ��������
	std::string owner;     // ��� �������� (�����)
	int type;              // ��� ������
	int state;             // ���������
	EWearFlag wear;              // ���� ���������
	int wear_message;      // ��� �������� ���� �����
	int weap_class;        // ����� ������
	int weap_message;      // ��� �������� ������
	int cost;              // ��� ����
	char cost_sign;        // ���� ���� +/-
	int  rent;             // ��� ��������� �����
	char rent_sign;        // ���� ����� +/-
	char new_timesign;	   // ���� ������� < > =
	time_t new_timedown;   // ������ ������� �������
	time_t new_timeup;	   // ������� ������� �������
	int filter_type;       // CLAN/EXCHANGE	
	
	std::vector<int> affect_apply; // ������� apply_types
	std::vector<int> affect_weap;  // ������� weapon_affects
	std::vector<int> affect_extra; // ������� extra_bits
	
	std::string show_obj_aff(OBJ_DATA *obj);

private:
	bool check_name(OBJ_DATA *obj, CHAR_DATA *ch = 0) const;
	bool check_type(OBJ_DATA *obj) const;
	bool check_state(OBJ_DATA *obj) const;
	bool check_wear(OBJ_DATA *obj) const;
	bool check_weap_class(OBJ_DATA *obj) const;
	bool check_cost(int obj_price) const;
	bool check_rent(int obj_price) const;
	bool check_affect_weap(OBJ_DATA *obj) const;
	bool check_affect_apply(OBJ_DATA *obj) const;
	bool check_affect_extra(OBJ_DATA *obj) const;
	bool check_owner(exchange_item_data *exch_obj) const;
	bool check_realtime(exchange_item_data *exch_obj) const;
};

int get_virtual_race(CHAR_DATA *mob);

#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)

#ifdef WIN32
class CCheckTable
{
public:
	typedef int(*original_t)(int);
	typedef bool(*table_t) (unsigned char);
	CCheckTable(original_t original, table_t table) : m_original(original), m_table(table) {}
	bool test_values() const;
	double test_time() const;
	void check() const;

private:
	original_t m_original;
	table_t m_table;
};
#endif

// global buffering system
#ifdef __DB_C__
char buf[MAX_STRING_LENGTH];
char buf1[MAX_STRING_LENGTH];
char buf2[MAX_STRING_LENGTH];
char arg[MAX_STRING_LENGTH];
#else
extern char buf[MAX_STRING_LENGTH];
extern char buf1[MAX_STRING_LENGTH];
extern char buf2[MAX_STRING_LENGTH];
extern char arg[MAX_STRING_LENGTH];
#endif

inline void graceful_exit(int retcode)
{
	_exit(retcode);
}

bool isname(const char *str, const char *namelist);
inline bool isname(const std::string &str, const char *namelist) { return isname(str.c_str(), namelist); }
inline bool isname(const char* str, const std::string& namelist) { return isname(str, namelist.c_str()); }
inline bool isname(const std::string &str, const std::string& namelist) { return isname(str.c_str(), namelist.c_str()); }

const char* one_word(const char* argument, char *first_arg);

void ReadEndString(std::ifstream &file);
// ������ ������� (� ������ ������ ����� ������) �� ���� ������, ��� ���������� ������� �������
void StringReplace(std::string& buffer, char s, const std::string& d);
std::string& format_news_message(std::string &text);

template <typename T>
void joinList(const T& list, std::string& result, const std::string& delimiter = ", ")
{
	std::stringstream ss;
	bool first = true;
	for (const auto& i : list)
	{
		ss << (first ? "" : delimiter) << i;
		first = false;
	}
	result = ss.str();
}

#endif // _UTILS_H_

// vim: ts=4 sw=4 tw=0 noet syntax=cpp :
