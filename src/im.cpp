/* ************************************************************************
*   File: im.cpp                                        Part of Bylins    *
*  Usage: Ingradient handling function                                    *
*                                                                         *
*                                                                         *
*  $Author$                                                        *
*  $Date$                                           *
*  $Revision$                                                       *
************************************************************************ */

/* ���������� ������������� ����� */

#include <stdio.h>
#include <stdlib.h>

#include "conf.h"
#include "sysdep.h"

#include "structs.h"
#include "utils.h"

#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "comm.h"
#include "constants.h"
#include "screen.h"

#include "im.h"

#define		VAR_CHAR	'@'

#define imlog(lvl,str)	mudlog(str, lvl, LVL_BUILDER, IMLOG, TRUE)

// �� spec_proc.c
char *how_good(CHAR_DATA * ch, int percent);


extern CHAR_DATA *mob_proto;
extern OBJ_DATA *obj_proto;
extern INDEX_DATA *obj_index;
extern INDEX_DATA *mob_index;
extern CHAR_DATA *character_list;

ACMD(do_rset);
ACMD(do_recipes);
ACMD(do_cook);
ACMD(do_imlist);

im_type *imtypes = NULL;	// ������ ������������������ �����/���������
int top_imtypes = -1;		// ��������� ����� ���� ��

im_recipe *imrecipes = NULL;	// ������ ������������������ �������� 
int top_imrecipes = -1;		// ��������� ����� ������� ��

/* ����� ���� �� ����� name. mode=0-������ ������������,1-��� ������ */
int im_get_type_by_name(char *name, int mode)
{
	int i;
	for (i = 0; i <= top_imtypes; ++i) {
		if (mode == 0 && imtypes[i].proto_vnum == -1)
			continue;
		if (!strn_cmp(name, imtypes[i].name, strlen(imtypes[i].name)))
			return i;
	}
	return -1;
}

// ����� rid �� id
int im_get_recipe(int id)
{
	int rid;
	for (rid = top_imrecipes; rid >= 0; --rid)
		if (imrecipes[rid].id == id)
			break;
	return rid;
}

// ����� rid �� �����
int im_get_recipe_by_name(char *name)
{
	int rid;
	int ok;
	char *temp, *temp2;
	char first[256], first2[256];

	if (*name == 0)
		return -1;

	for (rid = top_imrecipes; rid >= 0; --rid) {
		if (is_abbrev(name, imrecipes[rid].name))
			break;

		ok = TRUE;
		temp = any_one_arg(imrecipes[rid].name, first);
		temp2 = any_one_arg(name, first2);
		while (*first && *first2 && ok) {
			if (!is_abbrev(first2, first))
				ok = FALSE;
			temp = any_one_arg(temp, first);
			temp2 = any_one_arg(temp2, first2);
		}
		if (ok && !*first2)
			break;
	}
	return rid;
}

im_rskill *im_get_char_rskill(CHAR_DATA * ch, int rid)
{
	im_rskill *rs;
	for (rs = GET_RSKILL(ch); rs; rs = rs->link)
		if (rs->rid == rid)
			break;
	return rs;
}

int im_get_char_rskill_count(CHAR_DATA * ch)
{
	int cnt;
	im_rskill *rs;
	for (cnt = 0, rs = GET_RSKILL(ch); rs; rs = rs->link, ++cnt);
	return cnt;
}

// ��������� ��������� �����, �� ���������� ������� (� ������ �������������)
void TypeListAllocate(im_tlist * tl, int size)
{
	if (tl->size < size) {
		long *ptr;
		CREATE(ptr, long, size);
		if (tl->size) {
			memcpy(ptr, tl->types, size * 4);
			free(tl->types);
		}
		tl->size = size;
		tl->types = ptr;
	}
}

// ������������� � ������� ����� ������ ����� tl ��� ����� index
void TypeListSetSingle(im_tlist * tl, int index)
{
	TypeListAllocate(tl, (index >> 8) + 1);
	tl->types[index >> 8] |= (1 << (index & 31));
}

// ������������� � ������� ����� ������ ����� tl ��� ���� ������ src
void TypeListSet(im_tlist * tl, im_tlist * src)
{
	int i;
	TypeListAllocate(tl, src->size);
	for (i = 0; i < src->size; ++i)
		tl->types[i] |= src->types[i];
}

// ��������� � ������� ����� ������ ����� tl ��� index
int TypeListCheck(im_tlist * tl, int index)
{
	TypeListAllocate(tl, (index >> 8) + 1);
	return tl->types[index >> 8] & (1 << (index & 31));
}


// ������� ���� �����������, ��� �������� �� �����
const int def_probs[] = {
	20,			// 20%    - 1
	35,			// 15%    - 2
	45,			// 10%    - 3
	53,			//  8%    - 4
	60,			//  7%    - 5
	66,			//  6%    - 6
	71,			//  5%    - 7
	76,			//  5%    - 8
	80,			//  4%    - 9
	84,			//  4%    - 10
	88,			//  4%    - 11
	91,			//  3%    - 12
	94,			//  3%    - 13
	96,			//  2%    - 14
	98,			//  2%    - 15
	99,			//  1%    - 16
	100			//  1%    - 17
	    // ������ 17 ������ �� ����������
};
int im_calc_power(void)
{
	int j, i = number(1, 100);
	for (j = 0; i > def_probs[j++];);
	return j;
}


/*
   �������� ����������� ����������� ���������� � ��� �����:
     1. ������������ ��������� ��������� ������ �� ��������� ���� �����������
     2. ���������� ������� ����������� ����
*/

// ����� ������
char *get_im_alias(im_memb * s, char *name)
{
	char **al;
	for (al = s->aliases; al[0]; al += 2)
		if (!str_cmp(al[0], name))
			break;
	return al[1];
}

// ������� �������� alias � ��������� ������������
char *replace_alias(char *ptr, im_memb * sample, int rnum, char *std)
{
	char *dst, *al;
	char aname[16];

	if (!sample && rnum == -1)
		return ptr;

	// ����� ��������������� ������ � buf
	if (sample) {
		// ����� � �������
		if (std && (al = get_im_alias(sample, std)) != NULL) {
			ptr = al;
		} else {
			// ������������ ������ ������
			dst = buf;
			do {
				if (*ptr == VAR_CHAR) {
					int k;
					++ptr;
					for (k = 0; (*ptr) && a_isalnum(*ptr); aname[k++] = *ptr++);
					aname[k] = 0;
					al = get_im_alias(sample, aname);
					strcpy(dst, al ? al : aname);
					while (*dst != 0)
						++dst;
				}
			}
			while ((*dst++ = *ptr++) != 0);
			ptr = buf;
		}
	} else {
		// ����� � ���� (������ p0-p5)
		// ������������ ������ ������
		for (dst = buf; (*dst++ = *ptr++) != 0;) {
			int k;
			if (*ptr != VAR_CHAR)
				continue;
			sscanf(ptr + 1, "p%d", &k);
			if (k < 0 || k > 5)
				continue;
			ptr += 3;
			strcpy(dst, GET_PAD(mob_proto + rnum, k));
			while (*dst != 0)
				++dst;
		}
		ptr = buf;
	}

	return ptr;
}

int im_type_rnum(int vnum)
{
	int rind;
	for (rind = top_imtypes; rind >= 0; --rind)
		if (imtypes[rind].id == vnum)
			break;
	return rind;
}

char *def_alias[] = { "n0", "n1", "n2", "n3", "n4", "n5" };

// �������� ���� �����������
int im_assign_power(OBJ_DATA * obj)
/*
   obj - ����������� ����������

   � ������ ������ ���������� 0, ����� ������

   GET_OBJ_VAL(obj,IM_INDEX_SLOT) =        - ������� ����������� ��� VNUM ����
   GET_OBJ_VAL(obj,IM_TYPE_SLOT)  = index  - ����� ���� ����������� (�� im.lst)
   GET_OBJ_VAL(obj,IM_POWER_SLOT) = lev    - ������� �����������
*/
{
	int rind, onum;
	int rnum = -1;
	im_memb *sample, *p;
	int j;
	char *ptr;

// ������� ������ index � rnum
	rind = im_type_rnum(GET_OBJ_VAL(obj, IM_TYPE_SLOT));
	if (rind == -1)
		return 1;	// �������� ����� ���� �����������

// ����� ������� ��� ����
// ���� ������������ ����, �������� ������� �� ����
	onum = real_object(imtypes[rind].proto_vnum);
	if (onum < 0)
		return 4;
	if (GET_OBJ_VAL(&obj_proto[onum], 3) == IM_CLASS_JIV) {
		if (GET_OBJ_VAL(obj, IM_INDEX_SLOT) == -1)
			return 3;
		rnum = real_mobile(GET_OBJ_VAL(obj, IM_INDEX_SLOT));
		if (rnum < 0)
			return 3;	// �������� VNUM �������� ����
		GET_OBJ_VAL(obj, IM_POWER_SLOT) = GET_LEVEL(mob_proto + rnum);
	}
// ����������� ����� ��������� ����
	for (p = imtypes[rind].head, sample = NULL;
	     p && p->power <= GET_OBJ_VAL(obj, IM_POWER_SLOT); sample = p, p = p->next);

	if (sample)
		GET_OBJ_SEX(obj) = sample->sex;

// ������ ��������
// ������, ��������, alias
	for (j = 0; j < 6; ++j) {
		ptr = GET_OBJ_PNAME(&obj_proto[GET_OBJ_RNUM(obj)], j);
		if (GET_OBJ_PNAME(obj, j) != ptr)
			free(GET_OBJ_PNAME(obj, j));
		GET_OBJ_PNAME(obj, j) = str_dup(replace_alias(ptr, sample, rnum, def_alias[j]));
	}
	ptr = GET_OBJ_DESC(&obj_proto[GET_OBJ_RNUM(obj)]);
	if (GET_OBJ_DESC(obj) != ptr)
		free(GET_OBJ_DESC(obj));
	GET_OBJ_DESC(obj) = str_dup(replace_alias(ptr, sample, rnum, "s"));

	ptr = GET_OBJ_ALIAS(&obj_proto[GET_OBJ_RNUM(obj)]);
	if (GET_OBJ_ALIAS(obj) != ptr)
		free(GET_OBJ_ALIAS(obj));
	GET_OBJ_ALIAS(obj) = str_dup(replace_alias(ptr, sample, rnum, "a"));

	ptr = obj_proto[GET_OBJ_RNUM(obj)].name;
	if (obj->name != ptr)
		free(obj->name);
	obj->name = str_dup(replace_alias(ptr, sample, rnum, "m"));

// ��������� ������ ����� �������
// -- ���� �� ������� --

	return 0;
}


// �������� ����������� ����������� 
OBJ_DATA *load_ingredient(int index, int power, int rnum)
/*
   index - ����� ����������� ����������� ��� �������� (� imtypes[])
   power - ���� ����������� 
   rnum  - VNUM ����, � ������� �������� ����������
*/
{
	OBJ_DATA *ing;
	int err;

	while (1) {
		if (imtypes[index].proto_vnum < 0) {
			sprintf(buf, "IM METATYPE ingredient loading %d", imtypes[index].id);
			break;
		}
		ing = read_object(imtypes[index].proto_vnum, VIRTUAL);
		if (!ing) {
			sprintf(buf, "IM ingredient prototype %d not found", imtypes[index].proto_vnum);
			break;
		}

		GET_OBJ_VAL(ing, IM_INDEX_SLOT) = rnum;
		GET_OBJ_VAL(ing, IM_POWER_SLOT) = power;
		GET_OBJ_VAL(ing, IM_TYPE_SLOT) = imtypes[index].id;

		err = im_assign_power(ing);
		if (err != 0) {
			extract_obj(ing);
			sprintf(buf, "IM power assignment error %d", err);
			break;
		}

		return ing;
	}

	imlog(CMP, buf);
	return NULL;
}

void im_translate_rskill_to_id(void)
{
	CHAR_DATA *ch;
	im_rskill *rs;
	for (ch = character_list; ch; ch = ch->next) {
		if (IS_NPC(ch))
			continue;
		for (rs = GET_RSKILL(ch); rs; rs = rs->link)
			rs->rid = imrecipes[rs->rid].id;
	}
}

void im_translate_rskill_to_rid(void)
{
	CHAR_DATA *ch;
	im_rskill *rs, **prs;
	int rid;
	for (ch = character_list; ch; ch = ch->next) {
		if (IS_NPC(ch))
			continue;
		prs = &GET_RSKILL(ch);
		while ((rs = *prs) != NULL) {
			rid = im_get_recipe(rs->rid);
			if (rid >= 0) {
				rs->rid = rid;
				prs = &rs->link;
				continue;
			} else {
				*prs = rs->link;
				free(rs);
			}
		}
	}
}

void im_cleanup_type(im_type * t)
{
	free(t->name);
	if (t->tlst.size != 0) {
		free(t->tlst.types);
		t->tlst.size = 0;
	}
}

void im_cleanup_recipe(im_recipe * r)
{
	im_addon *a;
	free(r->name);
	free(r->require);
	free(r->msg_char[0]);
	free(r->msg_char[1]);
	free(r->msg_char[2]);
	free(r->msg_room[0]);
	free(r->msg_room[1]);
	free(r->msg_room[2]);
	while ((a = r->addon) != NULL) {
		r->addon = a->link;
		free(a);
	}
}

/* ������������� ���������� ������������� ����� */
void init_im(void)
{
	FILE *im_file;
	char tmp[1024], tlist[1024];
	im_memb *mbs, *mptr;
	int i, j;

	im_file = fopen(LIB_MISC "im.lst", "r");
	if (!im_file) {
		imlog(BRF, "Can not open im.lst");
		return;
	}
	// ������� �����, ��� ����, �� ������ reset
	// �������������� rid � �������
	im_translate_rskill_to_id();
	for (i = 0; i <= top_imtypes; ++i)
		im_cleanup_type(imtypes + i);
	for (i = 0; i <= top_imrecipes; ++i)
		im_cleanup_recipe(imrecipes + i);
	if (imtypes)
		free(imtypes);
	imtypes = NULL;
	top_imtypes = -1;
	if (imrecipes)
		free(imrecipes);
	imrecipes = NULL;
	top_imrecipes = -1;



	mbs = mptr = NULL;

	// ������ 1
	// ����������� ���������� �����/���������
	// ��������� ����������
	while (get_line(im_file, tmp)) {
		if (!strn_cmp(tmp, "���", 3) || !strn_cmp(tmp, "�������", 7))
			++top_imtypes;
		if (!strn_cmp(tmp, "������", 6))
			++top_imrecipes;
		if (!strn_cmp(tmp, "���", 3)) {
			if (mbs == NULL) {
				CREATE(mbs, im_memb, 1);
				mptr = mbs;
			} else {
				CREATE(mptr->next, im_memb, 1);
				mptr = mptr->next;
			}
			// ���������� ��� alias
			mptr->power = 0;
			while (get_line(im_file, tmp)) {
				if (*tmp == '~')
					break;
				mptr->power++;
			}
		}
	}

	// ��������� ������ ��� imtypes � ���������� �������
	CREATE(imtypes, im_type, top_imtypes + 1);
	top_imtypes = -1;

	// ��������� ������ ��� imrecipes � ���������� �������
	CREATE(imrecipes, im_recipe, top_imrecipes + 1);
	top_imrecipes = -1;

	// ������ 2
	rewind(im_file);
	while (get_line(im_file, tmp)) {
		int id, vnum;
		char dummy[128], name[128], text[1024];

		if (!strn_cmp(tmp, "���", 3)) {	// �������� ����
			if (sscanf(tmp, "%s %d %s %d", dummy, &id, name, &vnum) == 4) {
				++top_imtypes;
				imtypes[top_imtypes].id = id;
				imtypes[top_imtypes].name = str_dup(name);
				imtypes[top_imtypes].proto_vnum = vnum;
				imtypes[top_imtypes].head = NULL;
				imtypes[top_imtypes].tlst.size = 0;
				TypeListSetSingle(&imtypes[top_imtypes].tlst, top_imtypes);
				continue;
			}
			sprintf(text, "[IM] Invalid type : '%s'", tmp);
			imlog(NRM, text);
		} else if (!strn_cmp(tmp, "�������", 7)) {
			if (sscanf(tmp, "%s %d %s %s", dummy, &id, name, tlist) == 4) {
				char *p;
				++top_imtypes;
				imtypes[top_imtypes].id = id;
				imtypes[top_imtypes].name = str_dup(name);
				imtypes[top_imtypes].proto_vnum = -1;
				imtypes[top_imtypes].head = NULL;
				imtypes[top_imtypes].tlst.size = 0;
				for (p = strtok(tlist, ","); p; p = strtok(NULL, ",")) {
					int i = im_get_type_by_name(p, 1);	// ����� ������ ����
					if (i == -1) {
						sprintf(text, "[IM] Invalid type name : '%s'", p);
						imlog(NRM, text);
						continue;
					}
					TypeListSet(&imtypes[top_imtypes].tlst, &imtypes[i].tlst);
				}
				continue;
			}
			sprintf(text, "[IM] Invalid metatype : %s", tmp);
			imlog(NRM, text);
		} else if (!strn_cmp(tmp, "���", 3)) {
			int power, sex;
			mptr = mbs;
			mbs = mbs->next;
			if (sscanf(tmp, "%s %s %d %d", dummy, name, &power, &sex) == 4) {
				int i = im_get_type_by_name(name, 0);	// ����� ������������� ����
				if (i != -1) {
					char **p;
					im_memb *ins_after, *ins_before;
					CREATE(mptr->aliases, char *, 2 * (mptr->power + 1));
					mptr->power = power;
					mptr->sex = sex;
					p = mptr->aliases;
					while (get_line(im_file, tmp)) {
						if (*tmp == '~')
							break;
						sscanf(tmp, "%s %s", name, text);
						*p++ = str_dup(name);
						*p++ = str_dup(text);
					}
					p[0] = p[1] = NULL;
					// �������� � ��������� ���� �������� ����
					ins_after = NULL;
					ins_before = imtypes[i].head;
					while (ins_before && ins_before->power < mptr->power) {
						ins_after = ins_before;
						ins_before = ins_before->next;
					}
					if (ins_after == NULL)
						imtypes[i].head = mptr;
					else
						ins_after->next = mptr;
					mptr->next = ins_before;
					continue;
				}
				sprintf(text, "[IM] Can not find type : '%s'", name);
				imlog(NRM, text);
			}
			free(mptr);
			while (get_line(im_file, tmp))
				if (*tmp == '~')
					break;
			if (*tmp != '~') {
				sprintf(text, "[IM] Invalid inrgedient : '%s'", tmp);
				imlog(NRM, text);
			}
		} else if (!strn_cmp(tmp, "������", 6)) {
			char *p;
			// �������� �������
			if (sscanf(tmp, "%s %d %s", dummy, &id, name) == 3) {
				for (p = name; *p; ++p)
					if (*p == '_')
						*p = ' ';
				++top_imrecipes;
				imrecipes[top_imrecipes].id = id;
				imrecipes[top_imrecipes].name = str_dup(name);
				imrecipes[top_imrecipes].k_improove = 1000;
				while (get_line(im_file, tmp)) {
					if (*tmp == '~')
						break;
					if (!strn_cmp(tmp, "OBJ", 3)) {
						if (sscanf(tmp, "%s %d", dummy, &imrecipes[top_imrecipes].result) != 2)
							break;
					} else if (!strn_cmp(tmp, "IMP", 3)) {
						if (sscanf
						    (tmp, "%s %d", dummy, &imrecipes[top_imrecipes].k_improove) != 2)
							break;
					} else if (!strn_cmp(tmp, "CON", 3)) {
						if (sscanf(tmp, "%s %f %f %f %f",
							   dummy,
							   &imrecipes[top_imrecipes].k[0],
							   &imrecipes[top_imrecipes].k[1],
							   &imrecipes[top_imrecipes].k[2],
							   &imrecipes[top_imrecipes].kp) != 5)
							break;
					} else if (!strn_cmp(tmp, "MC1", 3)) {
						p = tmp + 3;
						skip_spaces(&p);
						if (imrecipes[top_imrecipes].msg_char[0])
							free(imrecipes[top_imrecipes].msg_char[0]);
						imrecipes[top_imrecipes].msg_char[0] = str_dup(p);
					} else if (!strn_cmp(tmp, "MR1", 3)) {
						p = tmp + 3;
						skip_spaces(&p);
						if (imrecipes[top_imrecipes].msg_room[0])
							free(imrecipes[top_imrecipes].msg_room[0]);
						imrecipes[top_imrecipes].msg_room[0] = str_dup(p);
					} else if (!strn_cmp(tmp, "MC2", 3)) {
						p = tmp + 3;
						skip_spaces(&p);
						if (imrecipes[top_imrecipes].msg_char[1])
							free(imrecipes[top_imrecipes].msg_char[1]);
						imrecipes[top_imrecipes].msg_char[1] = str_dup(p);
					} else if (!strn_cmp(tmp, "MR2", 3)) {
						p = tmp + 3;
						skip_spaces(&p);
						if (imrecipes[top_imrecipes].msg_room[1])
							free(imrecipes[top_imrecipes].msg_room[1]);
						imrecipes[top_imrecipes].msg_room[1] = str_dup(p);
					} else if (!strn_cmp(tmp, "MC3", 3)) {
						p = tmp + 3;
						skip_spaces(&p);
						if (imrecipes[top_imrecipes].msg_char[2])
							free(imrecipes[top_imrecipes].msg_char[2]);
						imrecipes[top_imrecipes].msg_char[2] = str_dup(p);
					} else if (!strn_cmp(tmp, "MR3", 3)) {
						p = tmp + 3;
						skip_spaces(&p);
						if (imrecipes[top_imrecipes].msg_room[2])
							free(imrecipes[top_imrecipes].msg_room[2]);
						imrecipes[top_imrecipes].msg_room[2] = str_dup(p);
					} else if (!strn_cmp(tmp, "DAM", 3)) {
						if (sscanf(tmp, "%s %dd%d",
							   dummy,
							   &imrecipes[top_imrecipes].x,
							   &imrecipes[top_imrecipes].y) != 3)
							break;
					} else if (!strn_cmp(tmp, "REQ", 3)) {
						im_parse(&imrecipes[top_imrecipes].require, tmp + 3);
					} else if (!strn_cmp(tmp, "ADD", 3)) {
						im_addon *adi;
						int n, k0, k1, k2, id;
						if (sscanf(tmp, "%s %d %s %d %d %d",
							   dummy, &n, name, &k0, &k1, &k2) != 6)
							break;
						id = im_get_type_by_name(name, 1);
						if (id < 0)
							break;
						while (n--) {
							CREATE(adi, im_addon, 1);
							adi->id = id;
							adi->k0 = k0;
							adi->k1 = k1;
							adi->k2 = k2;
							adi->obj = NULL;
							adi->link = imrecipes[top_imrecipes].addon;
							imrecipes[top_imrecipes].addon = adi;
						}
					}
				}
				if (*tmp == '~')
					continue;
			}
			sprintf(text, "[IM] Invalid recipe : '%s'", tmp);
			imlog(NRM, text);
		} else {
			if (*tmp) {
				sprintf(text, "[IM] Unrecognized command : '%s'", tmp);
				imlog(NRM, text);
			}
		}
	}

	fclose(im_file);

	// ����������� �������������� ������������ �����
	for (i = 0; i <= top_imtypes; ++i) {
		if (imtypes[i].proto_vnum == -1)
			continue;
		for (j = 0; j <= top_imtypes; ++j)
			if (i != j && imtypes[j].proto_vnum == -1 && TypeListCheck(&imtypes[j].tlst, i))
				TypeListSetSingle(&imtypes[i].tlst, j);
	}
	// �������� ������� ��� ��������� �����
	for (i = 0; i <= top_imtypes; ++i) {
		if (imtypes[i].proto_vnum != -1)
			continue;
		imtypes[i].tlst.size = 0;
		free(imtypes[i].tlst.types);
	}

	im_translate_rskill_to_rid();

#if 0
	log(NRM, "IM types dump");
	for (i = 0; i <= top_imtypes; ++i) {
		int j;
		log("RNUM=%d,ID=%d,NAME: %s", i, imtypes[i].id, imtypes[i].name);
		for (j = 0; j < imtypes[i].tlst.size; ++j)
			log("%08lX", imtypes[i].tlst.types[j]);
	}
	log("IM recipes dump");
	for (i = 0; i <= top_imrecipes; ++i) {
		log("RNUM=%d,ID=%d,NAME: %s", i, imrecipes[i].id, imrecipes[i].name);
	}
#endif

}

// ������������� ������ line � ��������� ����������� ������������
// ������ ������: <�����>:<���-��>
void im_parse(int **ing_list, char *line)
{
	int local_count = 0;
	int count = 0;
	int *local_list = NULL;
	int *res;
	int n, l, p, *ptr;

	while (1) {
		skip_spaces(&line);
		if (*line == 0)
			break;
		if (a_isdigit(*line)) {
			n = strtol(line, &line, 10);
			n = im_type_rnum(n);
		} else {
			n = im_get_type_by_name(line, 1);
			if (n >= 0)
				line += strlen(imtypes[n].name);
		}
		if (n < 0)
			break;
		l = 0xFFFF;
		if (*line == ',') {
			++line;
			l = strtol(line, &line, 10);
		}
		if (*line++ != ':')
			break;
		p = strtol(line, &line, 10);
		if (!p)
			break;
		if (!local_count)
			CREATE(local_list, int, 2);
		else
			RECREATE(local_list, int, local_count + 2);
		local_list[local_count++] = n;
		local_list[local_count++] = (l << 16) | p;
	}
	if (*ing_list) {
		for (ptr = *ing_list; (*ptr++ != -1););
		count = ptr - *ing_list - 1;
	}
	CREATE(res, int, local_count + count + 1);
	if (count)
		memcpy(res, *ing_list, count * sizeof(int));
	memcpy(res + count, local_list, local_count * sizeof(int));
	res[count + local_count] = -1;
	if (*ing_list)
		free(*ing_list);
	free(local_list);
	*ing_list = res;
}

// ������������ �������
// ������ ������ �����������, ��������� �����
void im_reset_room(ROOM_DATA * room)
{
	OBJ_DATA *o, *next;
	int indx;

	for (o = room->contents; o; o = next) {
		next = o->next_content;
		if (GET_OBJ_TYPE(o) == ITEM_MING)
			extract_obj(o);
	}

	if (!room->ing_list)
		return;		// ��������� ������

	for (indx = 0; room->ing_list[indx] != -1; indx += 2) {
		int power;
		if (number(1, 100) >= (room->ing_list[indx + 1] & 0xFFFF))
			continue;
		// ��������� ���������� � �������    
		power = (room->ing_list[indx + 1] >> 16) & 0xFFFF;
		if (power == 0xFFFF)
			power = im_calc_power();
		o = load_ingredient(room->ing_list[indx], power, -1);
		if (o)
			obj_to_room(o, real_room(room->number));
	}

}

// �������� �����
// ��������� ����������� � ����
void im_make_corpse(OBJ_DATA * corpse, int *ing_list)
{
	OBJ_DATA *o;
	int indx;

	for (indx = 0; ing_list[indx] != -1; indx += 2) {
		int power;
		if (number(1, 100) >= (ing_list[indx + 1] & 0xFFFF))
			continue;
		// ��������� ���������� � ����    
		power = (ing_list[indx + 1] >> 16) & 0xFFFF;
		if (power == 0xFFFF)
			power = im_calc_power();
		o = load_ingredient(ing_list[indx], power, GET_OBJ_VAL(corpse, 2));
		if (o)
			obj_to_obj(o, corpse);
	}
}


void list_recipes(CHAR_DATA * ch)
{
	int i = 0;
	im_rskill *rs = GET_RSKILL(ch);

	sprintf(buf, "�� �������� ���������� ��������� :\r\n");

	strcpy(buf2, buf);

	while (rs) {
		if (strlen(buf2) >= MAX_STRING_LENGTH - 60) {
			strcat(buf2, "**OVERFLOW**\r\n");
			break;
		}
		if (rs->perc <= 0)
			continue;
		sprintf(buf, "%-30s %s\r\n", imrecipes[rs->rid].name, how_good(ch, rs->perc));
		strcat(buf2, buf);
		++i;
		rs = rs->link;
	}

	if (!i)
		sprintf(buf2 + strlen(buf2), "��� ��������.\r\n");

	page_string(ch->desc, buf2, 1);
}

ACMD(do_recipes)
{
	if (IS_NPC(ch))
		return;
	list_recipes(ch);
}

ACMD(do_rset)
{
	CHAR_DATA *vict;
	char name[MAX_INPUT_LENGTH], buf2[128];
	char buf[MAX_INPUT_LENGTH], help[MAX_STRING_LENGTH];
	int rcpt = -1, value, i, qend;
	im_rskill *rs;

	argument = one_argument(argument, name);

	/*
	 * No arguments. print an informative text.
	 */
	if (!*name) {
		send_to_char("������: rset <�����> '<������>' <��������>\r\n", ch);
		strcpy(help, "������������������ �������:\r\n");
		for (qend = 0, i = 0; i <= top_imrecipes; i++) {
			sprintf(help + strlen(help), "%30s", imrecipes[i].name);
			if (qend++ % 2 == 1) {
				strcat(help, "\r\n");
				send_to_char(help, ch);
				*help = '\0';
			}
		}
		if (*help)
			send_to_char(help, ch);
		send_to_char("\r\n", ch);
		return;
	}

	if (!(vict = get_char_vis(ch, name, FIND_CHAR_WORLD))) {
		send_to_char(NOPERSON, ch);
		return;
	}
	skip_spaces(&argument);

	/* If there is no chars in argument */
	if (!*argument) {
		send_to_char("��������� �������� �������.\r\n", ch);
		return;
	}
	if (*argument != '\'') {
		send_to_char("������ ���� ��������� � ������� : ''\r\n", ch);
		return;
	}
	/* Locate the last quote and lowercase the magic words (if any) */

	for (qend = 1; argument[qend] && argument[qend] != '\''; qend++)
		argument[qend] = LOWER(argument[qend]);

	if (argument[qend] != '\'') {
		send_to_char("������ ������ ���� �������� � ������� : ''\r\n", ch);
		return;
	}
	strcpy(help, (argument + 1));
	help[qend - 1] = '\0';

	rcpt = im_get_recipe_by_name(help);

	if (rcpt < 0) {
		send_to_char("����������� ������.\r\n", ch);
		return;
	}
	argument += qend + 1;	/* skip to next parameter */
	argument = one_argument(argument, buf);

	if (!*buf) {
		send_to_char("�������� ������� �������.\r\n", ch);
		return;
	}
	value = atoi(buf);
	if (value < 0) {
		send_to_char("����������� �������� ������� 0.\r\n", ch);
		return;
	}
	if (value > 200) {
		send_to_char("������������ �������� ������� 200.\r\n", ch);
		return;
	}
	if (IS_NPC(vict)) {
		send_to_char("�� �� ������ �������� ������ ��� �����.\r\n", ch);
		return;
	}
	// ������ - ����� ������ rcpt � ���������� ��� � value
	rs = im_get_char_rskill(vict, rcpt);
	if (!rs) {
		CREATE(rs, im_rskill, 1);
		rs->rid = rcpt;
		rs->link = GET_RSKILL(vict);
		GET_RSKILL(vict) = rs;
	}
	rs->perc = value;

	sprintf(buf2, "%s changed %s's %s to %d.", GET_NAME(ch), GET_NAME(vict), imrecipes[rcpt].name, value);
	mudlog(buf2, BRF, -1, SYSLOG, TRUE);
	imm_log("%s changed %s's %s to %d.", GET_NAME(ch), GET_NAME(vict), imrecipes[rcpt].name, value);
	send_to_char(buf2, ch);
}

void im_improove_recipe(CHAR_DATA * ch, im_rskill * rs, int success)
{
	int i, n, prob, div, diff;

	if (IS_NPC(ch))
		return;

	if (IS_IMMORTAL(ch) ||
	    (IN_ROOM(ch) != NOWHERE &&
	     (diff = wis_app[GET_REAL_WIS(ch)].max_learn_l20 *
	      GET_LEVEL(ch) / 20 - rs->perc) > 0 && rs->perc < MAX_EXP_PERCENT + GET_REMORT(ch) * 5)) {
		for (i = 0, n = 0; i <= MAX_SKILLS; ++i)
			if (GET_SKILL(ch, i))
				++n;
		n = (n + 1) >> 1;
		n += im_get_char_rskill_count(ch);
		prob = success ? 20000 : 15000;
		div = int_app[GET_REAL_INT(ch)].improove;
		div += imrecipes[rs->rid].k_improove / 100;
		prob /= (MAX(1, div));
		if ((diff = n - wis_app[GET_REAL_WIS(ch)].max_skills) < 0)
			prob += (5 * diff);
		else
			prob += (10 * diff);
		prob += number(1, rs->perc * 5);
		if (number(1, MAX(1, prob)) <= GET_REAL_INT(ch)) {
			if (success)
				sprintf(buf,
					"%s�� �������� �������� ������������� ������� \"%s\".%s\r\n",
					CCICYN(ch, C_NRM), imrecipes[rs->rid].name, CCNRM(ch, C_NRM));
			else
				sprintf(buf,
					"%s������� ��������� ��� �������� �������� ������������� ������� \"%s\".%s\r\n",
					CCICYN(ch, C_NRM), imrecipes[rs->rid].name, CCNRM(ch, C_NRM));
			send_to_char(buf, ch);
			rs->perc += number(1, 2);
			if (!IS_IMMORTAL(ch))
				rs->perc = MIN(MAX_EXP_PERCENT + GET_REMORT(ch) * 5, rs->perc);
		}
	}
}

OBJ_DATA **im_obtain_ingredients(CHAR_DATA * ch, char *argument, int *count)
{
	char name[MAX_STRING_LENGTH], buf[128];
	OBJ_DATA **array = NULL;
	OBJ_DATA *o;
	int i, n = 0;

	while (1) {
		argument = one_argument(argument, name);
		if (!*name) {
			if (!n) {
				send_to_char("������� ���������� ����������� ��� �������.\r\n", ch);
			}
			count[0] = n;
			return array;
		}
		o = get_obj_in_list_vis(ch, name, ch->carrying);
		if (!o) {
			sprintf(buf, "� ��� ��� %s.\r\n", name);
			break;
		}
		if (GET_OBJ_TYPE(o) != ITEM_MING) {
			sprintf(buf, "�� ������ ������������ ������ ���������� �����������.\r\n");
			break;
		}
		if (im_type_rnum(GET_OBJ_VAL(o, IM_TYPE_SLOT)) < 0) {
			sprintf(buf, "���������� ���� %s �������, ������, ������������.\r\n", GET_OBJ_PNAME(o, 1));
			break;
		}
		for (i = 0; i < n; ++i) {
			if (array[i] != o)
				continue;
			sprintf(buf, "���� � ��� �� ���������� ������ ������������ ������.\r\n");
			break;
		}
		if (i != n)
			break;
		if (!array)
			CREATE(array, OBJ_DATA *, 1);
		else
			RECREATE(array, OBJ_DATA *, n + 1);
		array[n++] = o;
	}
	if (array)
		free(array);
	imlog(NRM, buf);
	send_to_char(buf, ch);
	return NULL;
}

#define		IS_RECIPE_DELIM(c)		(((c)=='\'')||((c)=='*')||((c)=='!'))

// ���������� �������
// ������ '������' <�����������>
ACMD(do_cook)
{
	char name[MAX_STRING_LENGTH];
	int rcpt = -1, qend, mres;
	im_rskill *rs;
	OBJ_DATA **objs;
	int tgt;
	int i, num, add_start;
	int *req;
	float W1, W2, osr, prob;
	float param[IM_NPARAM];
	int val[IM_NPARAM];
	im_addon *addon;

	// ����������, ��� �� ������ �������� ������
	skip_spaces(&argument);
	if (!*argument) {
		send_to_char("��������� �������� �������.\r\n", ch);
		return;
	}
	if (!IS_RECIPE_DELIM(*argument)) {
		send_to_char("������ ���� ��������� � ������� : ' * ��� !\r\n", ch);
		return;
	}
	for (qend = 1; argument[qend] && !IS_RECIPE_DELIM(argument[qend]); qend++)
		argument[qend] = LOWER(argument[qend]);
	if (!IS_RECIPE_DELIM(argument[qend])) {
		send_to_char("������ ������ ���� �������� � ������� : ' * ��� !\r\n", ch);
		return;
	}
	strcpy(name, (argument + 1));
	argument += qend + 1;
	name[qend - 1] = '\0';
	rcpt = im_get_recipe_by_name(name);
	if (rcpt < 0) {
		send_to_char("��� ����� ������ ����������.\r\n", ch);
		return;
	}
	rs = im_get_char_rskill(ch, rcpt);
	if (!rs) {
		send_to_char("��� ����� ������ ����������.\r\n", ch);
		return;
	}
	// rs - ������������ ������
	// argument - ������ ������������

	sprintf(name, "%s ���������� ������ %s", GET_NAME(ch), imrecipes[rs->rid].name);
	imlog(BRF, name);

	// �������������� ������ ���������� � ������ �������� 
	// � ��������� ��������� � �.�.
	objs = im_obtain_ingredients(ch, argument, &num);
	if (!objs)
		return;

	imlog(NRM, "������������ �����������:");
	name[0] = 0;
	for (i = 0; i < num; ++i) {
		sprintf(name + strlen(name), "%s:%d ",
			imtypes[im_type_rnum(GET_OBJ_VAL(objs[i], IM_TYPE_SLOT))].
			name, GET_OBJ_VAL(objs[i], IM_POWER_SLOT));
	}
	imlog(NRM, name);

	imlog(NRM, "���� ��������� ������� �����������");

	W1 = 0;
	W2 = 0;
	osr = 0;
	// ���� 1. �������� ����������
	i = 0;
	req = imrecipes[rs->rid].require;
	while (*req != -1) {
		int itype, ktype, osk;
		if (i == num) {
			imlog(NRM, "�� ������� �������� ������������");
			send_to_char("������, ��� �� ������� ������������.\r\n", ch);
			free(objs);
			return;
		}
		ktype = *req++;
		osk = *req++ & 0xFFFF;
		osr += osk;
		sprintf(name, "����������� ����������: type=%d,osk=%d", ktype, osk);
		imlog(CMP, name);
		itype = im_type_rnum(GET_OBJ_VAL(objs[i], IM_TYPE_SLOT));
		// ktype - ��������� ���, itype - ������������� ���
		if (!TypeListCheck(&imtypes[itype].tlst, ktype)) {
			imlog(NRM, "����������� ����������");
			send_to_char("������, �� ���������� �����������.\r\n", ch);
			free(objs);
			return;
		}
		itype = GET_OBJ_VAL(objs[i], IM_POWER_SLOT);
		if (itype > osk)
			W1 += (itype - osk) * osk;
		else
			W2 += osk - itype;
		++i;
	}
	add_start = i;
	if (osr)
		W1 /= osr;
	sprintf(name, "������������ �������� �����������: W1=%f W2=%f", W1, W2);
	imlog(CMP, name);
	// �������������� ���������� ���������
	tgt = real_object(imrecipes[rs->rid].result);
	if (tgt < 0) {
		imlog(NRM, "�������� ������");
		send_to_char("��������� ������� ������.\r\n", ch);
		free(objs);
		return;
	}

	switch (GET_OBJ_TYPE(obj_proto + tgt)) {
	case ITEM_SCROLL:
	case ITEM_POTION:
		param[0] = GET_OBJ_VAL(obj_proto + tgt, 0);	// �������
		param[1] = 1;	// ����������
		param[2] = GET_OBJ_TIMER(obj_proto + tgt);	// ������
		break;
	case ITEM_WAND:
	case ITEM_STAFF:
		param[0] = GET_OBJ_VAL(obj_proto + tgt, 0);	// �������
		param[1] = GET_OBJ_VAL(obj_proto + tgt, 1);	// ����������
		param[2] = GET_OBJ_TIMER(obj_proto + tgt);	// ������
		break;
	default:
		imlog(NRM, "�������� ����� �������� ���");
		send_to_char("��������� ������ �������������.\r\n", ch);
		free(objs);
		return;
	}

	sprintf(name,
		"������� ��������� � ���� �������� � ���������� ��: %f,%f %f,%f %f,%f",
		param[0], imrecipes[rs->rid].k[0],
		param[1], imrecipes[rs->rid].k[1], param[2], imrecipes[rs->rid].k[2]);
	imlog(CMP, name);

	W2 *= imrecipes[rs->rid].kp;
	prob = (float) rs->perc - 5 + 2 * W1 - W2;
	for (i = 0; i < IM_NPARAM; ++i) {
		param[i] *= imrecipes[rs->rid].k[i];
		W1 += param[i];
	}

	imlog(CMP, "���� ������� �� �������� ������������:");
	sprintf(name, "�����������: %f", prob);
	imlog(CMP, name);
	sprintf(name, "����� ���������� ���: x+y+z=%f", W1);
	imlog(CMP, name);
	sprintf(name, "������������ �����������: x0=%f y0=%f z0=%f", param[0], param[1], param[2]);
	imlog(CMP, name);

	if (prob < 0) {
		send_to_char("� ������������� ������ �������� ��� ����� ���� �� ��������...\r\n", ch);
		free(objs);
		return;
	}

	// ���� 2. �������������� ����������
	for (addon = imrecipes[rs->rid].addon; addon; addon = addon->link)
		addon->obj = NULL;
	for (i = add_start; i < num; ++i) {
		int itype = im_type_rnum(GET_OBJ_VAL(objs[i], IM_TYPE_SLOT));
		for (addon = imrecipes[rs->rid].addon; addon; addon = addon->link)
			if (addon->obj == NULL && TypeListCheck(&imtypes[itype].tlst, addon->id))
				break;
		if (addon) {
			// "�����" ������
			int s = addon->k0 + addon->k1 + addon->k2;
			addon->obj = objs[i];
			param[0] += (float) GET_OBJ_VAL(objs[i], IM_POWER_SLOT) * addon->k0 / s;
			param[1] += (float) GET_OBJ_VAL(objs[i], IM_POWER_SLOT) * addon->k1 / s;
			param[2] += (float) GET_OBJ_VAL(objs[i], IM_POWER_SLOT) * addon->k2 / s;
		} else {
			// "������" ������
			W1 -= GET_OBJ_VAL(objs[i], IM_POWER_SLOT);
		}
	}

	sprintf(name, "����� ���������� ��� ����� ��������: x+y+z=%f", W1);
	imlog(CMP, name);
	sprintf(name, "������������ ����������� ����� ��������: x0=%f y0=%f z0=%f", param[0], param[1], param[2]);
	imlog(CMP, name);

	// ���� 3. ��������� ����������
	for (W2 = 0, i = 0; i < IM_NPARAM; ++i)
		W2 += param[i];
	for (i = 0; i < IM_NPARAM; ++i) {
		param[i] *= W1;
		param[i] /= W2;
	}
	for (i = 0; i < IM_NPARAM; ++i) {
		if (imrecipes[rs->rid].k[i])
			val[i] = (int) (0.5 + param[i] / imrecipes[rs->rid].k[i]);
		else
			val[i] = -1;	// �� ��������
	}

	sprintf(name, "��������� ����������: %d %d %d", val[0], val[1], val[2]);
	imlog(CMP, name);

	// ������� �������
	for (i = 0; i < num; ++i)
		extract_obj(objs[i]);
	free(objs);

	imlog(CMP, "����������� �������");

	// ������ ������ �� ��������
	mres = number(1, 100);
	if (mres < (int) prob)
		mres = IM_MSG_OK;
	else {
		mres = (100 - (int) prob) / 2;
		if (mres >= number(1, 100))
			mres = IM_MSG_FAIL;
		else
			mres = IM_MSG_DAM;
	}

	sprintf(name, "����� - %d", mres);
	imlog(CMP, name);

	im_improove_recipe(ch, rs, mres == IM_MSG_OK);

	// �������� ���������
	imlog(CMP, "�������� ���������");
	act(imrecipes[rs->rid].msg_char[mres], TRUE, ch, 0, 0, TO_CHAR);
	act(imrecipes[rs->rid].msg_room[mres], TRUE, ch, 0, 0, TO_ROOM);

	if (mres == IM_MSG_OK) {
		OBJ_DATA *result;
		imlog(CMP, "�������� ����������");
		result = read_object(tgt, REAL);
		if (result) {
			switch (GET_OBJ_TYPE(result)) {
			case ITEM_SCROLL:
			case ITEM_POTION:
				if (val[0] > 0)
					GET_OBJ_VAL(result, 0) = val[0];
				if (val[2] > 0)
					GET_OBJ_TIMER(result) = val[2];
				break;
			case ITEM_WAND:
			case ITEM_STAFF:
				if (val[0] > 0)
					GET_OBJ_VAL(result, 0) = val[0];
				if (val[1] > 0)
					GET_OBJ_VAL(result, 1) = GET_OBJ_VAL(result, 2) = val[1];
				if (val[2] > 0)
					GET_OBJ_TIMER(result) = val[2];
				break;
			}
			obj_to_char(result, ch);
		}
	}

/*
  if ( mres == IM_MSG_DAM )
  {
    dam = dice(imrecipes[rs->rid].x,imrecipes[rs->rid].y);
  }
*/

	return;
}

int im_ing_dump(int *ping, char *s)
{
	int pow;
	if (!ping || *ping == -1)
		return 0;
	pow = (ping[1] >> 16) & 0xFFFF;
	if (pow != 0xFFFF)
		sprintf(s, "%s,%d:%d", imtypes[ping[0]].name, pow, ping[1] & 0xFFFF);
	else
		sprintf(s, "%s:%d", imtypes[ping[0]].name, ping[1] & 0xFFFF);
	return 1;
}

void im_inglist_copy(int **pdst, int *src)
{
	int i;
	*pdst = NULL;
	if (!src)
		return;
	for (i = 0; src[i] != -1; i += 2);
	++i;
	CREATE(*pdst, int, i);
	memcpy(*pdst, src, i * sizeof(int));
	return;
}

void im_inglist_save_to_disk(FILE * f, int *ping)
{
	char str[128];
	for (; im_ing_dump(ping, str); ping += 2)
		fprintf(f, "I %s\r\n", str);
}

void im_extract_ing(int **pdst, int num)
{
	int *p1, *p2;
	int i;
	if (!*pdst)
		return;
	p1 = p2 = *pdst;
	i = 0;
	while (*p1 != -1) {
		if (i != num) {
			p2[0] = p1[0];
			p2[1] = p1[1];
			p2 += 2;
		}
		++i;
		p1 += 2;
	}
	*p2 = *p1;
	if (**pdst == -1) {
		free(*pdst);
		*pdst = NULL;
	}
}

void trg_recipeturn(CHAR_DATA * ch, int rid, int recipediff)
{
	im_rskill *rs;
	rs = im_get_char_rskill(ch, rid);
	if (rs) {
		if (recipediff)
			return;
		sprintf(buf, "��� ������ ������� '%s'.\r\n", imrecipes[rid].name);
		send_to_char(buf, ch);
		rs->perc = 0;
	} else {
		if (!recipediff)
			return;
		CREATE(rs, im_rskill, 1);
		rs->rid = rid;
		rs->link = GET_RSKILL(ch);
		GET_RSKILL(ch) = rs;
		rs->perc = 5;
		sprintf(buf, "�� ������� ������ '%s'.\r\n", imrecipes[rid].name);
		send_to_char(buf, ch);
	}
}

void trg_recipeadd(CHAR_DATA * ch, int rid, int recipediff)
{
	im_rskill *rs;
	int skill;

	rs = im_get_char_rskill(ch, rid);
	if (!rs)
		return;

	skill = rs->perc;
	rs->perc = MAX(1, MIN(skill + recipediff, 200));

	if (skill > rs->perc)
		sprintf(buf, "���� ������ ������� '%s' ����������.\r\n", imrecipes[rid].name);
	else if (skill < rs->perc)
		sprintf(buf, "�� �������� ������ ������� '%s'.\r\n", imrecipes[rid].name);
	else
		sprintf(buf, "���� ������ ������� '%s' �������� ����������.\r\n", imrecipes[rid].name);
	send_to_char(buf, ch);
}

ACMD(do_imlist)
{
	int zone, i, rnum;
	int *ping;
	char *str;

	one_argument(argument, buf);
	if (!*buf) {
		send_to_char("�������������: ������� <����� ����>\r\n", ch);
		return;
	}

	zone = atoi(buf);

	if ((zone < 0) || (zone > 999)) {
		send_to_char("����� ���� ������ ���� ����� 0 � 999.\n\r", ch);
		return;
	}

	buf[0] = 0;

	for (i = 0; i < 100; ++i) {
		if ((rnum = real_room(i + 100 * zone)) == NOWHERE)
			continue;
		ping = world[rnum]->ing_list;
		for (str = buf1, str[0] = 0; im_ing_dump(ping, str); ping += 2) {
			strcat(str, " ");
			str += strlen(str);
		}
		if (buf1[0]) {
			sprintf(buf + strlen(buf), "������� %d [%s]\r\n%s\r\n",
				world[rnum]->number, world[rnum]->name, buf1);
		}
	}

	for (i = 0; i < 100; ++i) {
		if ((rnum = real_mobile(i + 100 * zone)) == NOWHERE)
			continue;
		ping = mob_proto[rnum].ing_list;
		for (str = buf1, str[0] = 0; im_ing_dump(ping, str); ping += 2) {
			strcat(str, " ");
			str += strlen(str);
		}
		if (buf1[0]) {
			sprintf(buf + strlen(buf), "��� %d [%s,%d]\r\n%s\r\n",
				GET_MOB_VNUM(mob_proto + rnum),
				GET_NAME(mob_proto + rnum), GET_LEVEL(mob_proto + rnum), buf1);
		}
	}

	if (!buf[0])
		send_to_char("� ���� ����������� �� �����������", ch);
	else
		page_string(ch->desc, buf, 1);

}
