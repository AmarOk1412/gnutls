/*   -*- buffer-read-only: t -*- vi: set ro:
 *  
 *  DO NOT EDIT THIS FILE   (danetool-args.c)
 *  
 *  It has been AutoGen-ed  October 12, 2012 at 05:34:29 PM by AutoGen 5.16
 *  From the definitions    danetool-args.def
 *  and the template file   options
 *
 * Generated from AutoOpts 36:4:11 templates.
 *
 *  AutoOpts is a copyrighted work.  This source file is not encumbered
 *  by AutoOpts licensing, but is provided under the licensing terms chosen
 *  by the danetool author or copyright holder.  AutoOpts is
 *  licensed under the terms of the LGPL.  The redistributable library
 *  (``libopts'') is licensed under the terms of either the LGPL or, at the
 *  users discretion, the BSD license.  See the AutoOpts and/or libopts sources
 *  for details.
 *
 * The danetool program is copyrighted and licensed
 * under the following terms:
 *
 *  Copyright (C) 2000-2012 Free Software Foundation, all rights reserved.
 *  This is free software. It is licensed for use, modification and
 *  redistribution under the terms of the
 *  GNU General Public License, version 3 or later
 *      <http://gnu.org/licenses/gpl.html>
 *
 *  danetool is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  danetool is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __doxygen__
#define OPTION_CODE_COMPILE 1
#include "danetool-args.h"
#include <sys/types.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef  __cplusplus
extern "C" {
#endif
extern FILE * option_usage_fp;

/* TRANSLATORS: choose the translation for option names wisely because you
                cannot ever change your mind. */
#define zCopyright      (danetool_opt_strs+0)
#define zLicenseDescrip (danetool_opt_strs+279)


#ifndef NULL
#  define NULL 0
#endif

/*
 *  danetool option static const strings
 */
static char const danetool_opt_strs[2268] =
/*     0 */ "danetool @VERSION@\n"
            "Copyright (C) 2000-2012 Free Software Foundation, all rights reserved.\n"
            "This is free software. It is licensed for use, modification and\n"
            "redistribution under the terms of the\n"
            "GNU General Public License, version 3 or later\n"
            "    <http://gnu.org/licenses/gpl.html>\n\0"
/*   279 */ "danetool is free software: you can redistribute it and/or modify it under\n"
            "the terms of the GNU General Public License as published by the Free\n"
            "Software Foundation, either version 3 of the License, or (at your option)\n"
            "any later version.\n\n"
            "danetool is distributed in the hope that it will be useful, but WITHOUT ANY\n"
            "WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS\n"
            "FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more\n"
            "details.\n\n"
            "You should have received a copy of the GNU General Public License along\n"
            "with this program.  If not, see <http://www.gnu.org/licenses/>.\n\0"
/*   884 */ "Enable debugging.\0"
/*   902 */ "DEBUG\0"
/*   908 */ "debug\0"
/*   914 */ "More verbose output\0"
/*   934 */ "VERBOSE\0"
/*   942 */ "verbose\0"
/*   950 */ "Input file\0"
/*   961 */ "INFILE\0"
/*   968 */ "infile\0"
/*   975 */ "Output file\0"
/*   987 */ "OUTFILE\0"
/*   995 */ "outfile\0"
/*  1003 */ "Loads a public key file\0"
/*  1027 */ "LOAD_PUBKEY\0"
/*  1039 */ "load-pubkey\0"
/*  1051 */ "Loads a certificate file\0"
/*  1076 */ "LOAD_CERTIFICATE\0"
/*  1093 */ "load-certificate\0"
/*  1110 */ "Hash algorithm to use for signing.\0"
/*  1145 */ "HASH\0"
/*  1150 */ "hash\0"
/*  1155 */ "Use DER format for input certificates and private keys.\0"
/*  1211 */ "INDER\0"
/*  1217 */ "no-inder\0"
/*  1226 */ "no\0"
/*  1229 */ "This is an alias for 'inder'\0"
/*  1258 */ "inraw\0"
/*  1264 */ "Use DER format for output certificates and private keys\0"
/*  1320 */ "OUTDER\0"
/*  1327 */ "no-outder\0"
/*  1337 */ "This is an alias for 'outder'\0"
/*  1367 */ "outraw\0"
/*  1374 */ "Print the DANE RR data on a certificate or public key\0"
/*  1428 */ "TLSA_RR\0"
/*  1436 */ "tlsa-rr\0"
/*  1444 */ "Specify the hostname to be used in the DANE RR\0"
/*  1491 */ "HOST\0"
/*  1496 */ "host\0"
/*  1501 */ "The protocol set for DANE data (tcp, udp etc.)\0"
/*  1548 */ "PROTO\0"
/*  1554 */ "proto\0"
/*  1560 */ "Specify the port number for the DANE data.\0"
/*  1603 */ "PORT\0"
/*  1608 */ "port\0"
/*  1613 */ "Whether the provided certificate or public key is a Certificate\n"
            "authority.\0"
/*  1688 */ "CA\0"
/*  1691 */ "ca\0"
/*  1694 */ "Use the hash of the X.509 certificate, rather than the public key.\0"
/*  1761 */ "X509\0"
/*  1766 */ "x509\0"
/*  1771 */ "The provided certificate or public key is a local entity.\0"
/*  1829 */ "LOCAL\0"
/*  1835 */ "local\0"
/*  1841 */ "Display extended usage information and exit\0"
/*  1885 */ "help\0"
/*  1890 */ "Extended usage information passed thru pager\0"
/*  1935 */ "more-help\0"
/*  1945 */ "Output version information and exit\0"
/*  1981 */ "version\0"
/*  1989 */ "DANETOOL\0"
/*  1998 */ "danetool - GnuTLS DANE tool - Ver. @VERSION@\n"
            "USAGE:  %s [ -<flag> [<val>] | --<name>[{=| }<val>] ]...\n\0"
/*  2101 */ "bug-gnutls@gnu.org\0"
/*  2120 */ "\n\n\0"
/*  2123 */ "\n"
            "Tool generate DNS resource records for the DANE protocol.\n\0"
/*  2183 */ "danetool @VERSION@\0"
/*  2202 */ "danetool [options] [url]\n"
            "danetool --help for usage instructions.\n";

/*
 *  debug option description:
 */
#define DEBUG_DESC      (danetool_opt_strs+884)
#define DEBUG_NAME      (danetool_opt_strs+902)
#define DEBUG_name      (danetool_opt_strs+908)
#define DEBUG_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

/*
 *  verbose option description:
 */
#define VERBOSE_DESC      (danetool_opt_strs+914)
#define VERBOSE_NAME      (danetool_opt_strs+934)
#define VERBOSE_name      (danetool_opt_strs+942)
#define VERBOSE_FLAGS     (OPTST_DISABLED)

/*
 *  infile option description:
 */
#define INFILE_DESC      (danetool_opt_strs+950)
#define INFILE_NAME      (danetool_opt_strs+961)
#define INFILE_name      (danetool_opt_strs+968)
#define INFILE_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_FILE))

/*
 *  outfile option description:
 */
#define OUTFILE_DESC      (danetool_opt_strs+975)
#define OUTFILE_NAME      (danetool_opt_strs+987)
#define OUTFILE_name      (danetool_opt_strs+995)
#define OUTFILE_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  load-pubkey option description:
 */
#define LOAD_PUBKEY_DESC      (danetool_opt_strs+1003)
#define LOAD_PUBKEY_NAME      (danetool_opt_strs+1027)
#define LOAD_PUBKEY_name      (danetool_opt_strs+1039)
#define LOAD_PUBKEY_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  load-certificate option description:
 */
#define LOAD_CERTIFICATE_DESC      (danetool_opt_strs+1051)
#define LOAD_CERTIFICATE_NAME      (danetool_opt_strs+1076)
#define LOAD_CERTIFICATE_name      (danetool_opt_strs+1093)
#define LOAD_CERTIFICATE_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  hash option description:
 */
#define HASH_DESC      (danetool_opt_strs+1110)
#define HASH_NAME      (danetool_opt_strs+1145)
#define HASH_name      (danetool_opt_strs+1150)
#define HASH_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  inder option description:
 */
#define INDER_DESC      (danetool_opt_strs+1155)
#define INDER_NAME      (danetool_opt_strs+1211)
#define NOT_INDER_name  (danetool_opt_strs+1217)
#define NOT_INDER_PFX   (danetool_opt_strs+1226)
#define INDER_name      (NOT_INDER_name + 3)
#define INDER_FLAGS     (OPTST_DISABLED)

/*
 *  inraw option description:
 */
#define INRAW_DESC    (danetool_opt_strs+1229)
#define INRAW_NAME    NULL
#define INRAW_name    (danetool_opt_strs+1258)
#define INRAW_FLAGS     (INDER_FLAGS | OPTST_ALIAS)

/*
 *  outder option description:
 */
#define OUTDER_DESC      (danetool_opt_strs+1264)
#define OUTDER_NAME      (danetool_opt_strs+1320)
#define NOT_OUTDER_name  (danetool_opt_strs+1327)
#define NOT_OUTDER_PFX   (danetool_opt_strs+1226)
#define OUTDER_name      (NOT_OUTDER_name + 3)
#define OUTDER_FLAGS     (OPTST_DISABLED)

/*
 *  outraw option description:
 */
#define OUTRAW_DESC    (danetool_opt_strs+1337)
#define OUTRAW_NAME    NULL
#define OUTRAW_name    (danetool_opt_strs+1367)
#define OUTRAW_FLAGS     (OUTDER_FLAGS | OPTST_ALIAS)

/*
 *  tlsa-rr option description with
 *  "Must also have options" and "Incompatible options":
 */
#define TLSA_RR_DESC      (danetool_opt_strs+1374)
#define TLSA_RR_NAME      (danetool_opt_strs+1428)
#define TLSA_RR_name      (danetool_opt_strs+1436)
static int const aTlsa_RrMustList[] = {
    INDEX_OPT_HOST, NO_EQUIVALENT };
#define TLSA_RR_FLAGS     (OPTST_DISABLED)

/*
 *  host option description:
 */
#define HOST_DESC      (danetool_opt_strs+1444)
#define HOST_NAME      (danetool_opt_strs+1491)
#define HOST_name      (danetool_opt_strs+1496)
#define HOST_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  proto option description:
 */
#define PROTO_DESC      (danetool_opt_strs+1501)
#define PROTO_NAME      (danetool_opt_strs+1548)
#define PROTO_name      (danetool_opt_strs+1554)
#define PROTO_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_STRING))

/*
 *  port option description:
 */
#define PORT_DESC      (danetool_opt_strs+1560)
#define PORT_NAME      (danetool_opt_strs+1603)
#define PORT_name      (danetool_opt_strs+1608)
#define PORT_FLAGS     (OPTST_DISABLED \
        | OPTST_SET_ARGTYPE(OPARG_TYPE_NUMERIC))

/*
 *  ca option description:
 */
#define CA_DESC      (danetool_opt_strs+1613)
#define CA_NAME      (danetool_opt_strs+1688)
#define CA_name      (danetool_opt_strs+1691)
#define CA_FLAGS     (OPTST_DISABLED)

/*
 *  x509 option description:
 */
#define X509_DESC      (danetool_opt_strs+1694)
#define X509_NAME      (danetool_opt_strs+1761)
#define X509_name      (danetool_opt_strs+1766)
#define X509_FLAGS     (OPTST_DISABLED)

/*
 *  local option description:
 */
#define LOCAL_DESC      (danetool_opt_strs+1771)
#define LOCAL_NAME      (danetool_opt_strs+1829)
#define LOCAL_name      (danetool_opt_strs+1835)
#define LOCAL_FLAGS     (OPTST_DISABLED)

/*
 *  Help/More_Help/Version option descriptions:
 */
#define HELP_DESC       (danetool_opt_strs+1841)
#define HELP_name       (danetool_opt_strs+1885)
#ifdef HAVE_WORKING_FORK
#define MORE_HELP_DESC  (danetool_opt_strs+1890)
#define MORE_HELP_name  (danetool_opt_strs+1935)
#define MORE_HELP_FLAGS (OPTST_IMM | OPTST_NO_INIT)
#else
#define MORE_HELP_DESC  NULL
#define MORE_HELP_name  NULL
#define MORE_HELP_FLAGS (OPTST_OMITTED | OPTST_NO_INIT)
#endif
#ifdef NO_OPTIONAL_OPT_ARGS
#  define VER_FLAGS     (OPTST_IMM | OPTST_NO_INIT)
#else
#  define VER_FLAGS     (OPTST_SET_ARGTYPE(OPARG_TYPE_STRING) | \
                         OPTST_ARG_OPTIONAL | OPTST_IMM | OPTST_NO_INIT)
#endif
#define VER_DESC        (danetool_opt_strs+1945)
#define VER_name        (danetool_opt_strs+1981)
/*
 *  Declare option callback procedures
 */
extern tOptProc
    optionBooleanVal,   optionNestedVal,    optionNumericVal,
    optionPagedUsage,   optionPrintVersion, optionResetOpt,
    optionStackArg,     optionTimeDate,     optionTimeVal,
    optionUnstackArg,   optionVendorOption;
static tOptProc
    doOptDebug, doOptInfile, doOptInraw, doOptOutraw, doUsageOpt;
#define VER_PROC        optionPrintVersion

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 *  Define the danetool Option Descriptions.
 * This is an array of OPTION_CT entries, one for each
 * option that the danetool program responds to.
 */
static tOptDesc optDesc[OPTION_CT] = {
  {  /* entry idx, value */ 0, VALUE_OPT_DEBUG,
     /* equiv idx, value */ 0, VALUE_OPT_DEBUG,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ DEBUG_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --debug */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doOptDebug,
     /* desc, NAME, name */ DEBUG_DESC, DEBUG_NAME, DEBUG_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 1, VALUE_OPT_VERBOSE,
     /* equiv idx, value */ 1, VALUE_OPT_VERBOSE,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, NOLIMIT, 0,
     /* opt state flags  */ VERBOSE_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --verbose */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ VERBOSE_DESC, VERBOSE_NAME, VERBOSE_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 2, VALUE_OPT_INFILE,
     /* equiv idx, value */ 2, VALUE_OPT_INFILE,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ INFILE_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --infile */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doOptInfile,
     /* desc, NAME, name */ INFILE_DESC, INFILE_NAME, INFILE_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 3, VALUE_OPT_OUTFILE,
     /* equiv idx, value */ 3, VALUE_OPT_OUTFILE,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ OUTFILE_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --outfile */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ OUTFILE_DESC, OUTFILE_NAME, OUTFILE_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 4, VALUE_OPT_LOAD_PUBKEY,
     /* equiv idx, value */ 4, VALUE_OPT_LOAD_PUBKEY,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ LOAD_PUBKEY_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --load-pubkey */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ LOAD_PUBKEY_DESC, LOAD_PUBKEY_NAME, LOAD_PUBKEY_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 5, VALUE_OPT_LOAD_CERTIFICATE,
     /* equiv idx, value */ 5, VALUE_OPT_LOAD_CERTIFICATE,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ LOAD_CERTIFICATE_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --load-certificate */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ LOAD_CERTIFICATE_DESC, LOAD_CERTIFICATE_NAME, LOAD_CERTIFICATE_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 6, VALUE_OPT_HASH,
     /* equiv idx, value */ 6, VALUE_OPT_HASH,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ HASH_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --hash */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ HASH_DESC, HASH_NAME, HASH_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 7, VALUE_OPT_INDER,
     /* equiv idx, value */ 7, VALUE_OPT_INDER,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ INDER_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --inder */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ INDER_DESC, INDER_NAME, INDER_name,
     /* disablement strs */ NOT_INDER_name, NOT_INDER_PFX },

  {  /* entry idx, value */ 8, VALUE_OPT_INRAW,
     /* equiv idx, value */ 8, VALUE_OPT_INRAW,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ INRAW_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --inraw */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doOptInraw,
     /* desc, NAME, name */ INRAW_DESC, INRAW_NAME, INRAW_name,
     /* disablement strs */ 0, 0 },

  {  /* entry idx, value */ 9, VALUE_OPT_OUTDER,
     /* equiv idx, value */ 9, VALUE_OPT_OUTDER,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ OUTDER_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --outder */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ OUTDER_DESC, OUTDER_NAME, OUTDER_name,
     /* disablement strs */ NOT_OUTDER_name, NOT_OUTDER_PFX },

  {  /* entry idx, value */ 10, VALUE_OPT_OUTRAW,
     /* equiv idx, value */ 10, VALUE_OPT_OUTRAW,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ OUTRAW_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --outraw */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doOptOutraw,
     /* desc, NAME, name */ OUTRAW_DESC, OUTRAW_NAME, OUTRAW_name,
     /* disablement strs */ 0, 0 },

  {  /* entry idx, value */ 11, VALUE_OPT_TLSA_RR,
     /* equiv idx, value */ 11, VALUE_OPT_TLSA_RR,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ TLSA_RR_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --tlsa-rr */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ aTlsa_RrMustList, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ TLSA_RR_DESC, TLSA_RR_NAME, TLSA_RR_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 12, VALUE_OPT_HOST,
     /* equiv idx, value */ 12, VALUE_OPT_HOST,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ HOST_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --host */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ HOST_DESC, HOST_NAME, HOST_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 13, VALUE_OPT_PROTO,
     /* equiv idx, value */ 13, VALUE_OPT_PROTO,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ PROTO_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --proto */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ PROTO_DESC, PROTO_NAME, PROTO_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 14, VALUE_OPT_PORT,
     /* equiv idx, value */ 14, VALUE_OPT_PORT,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ PORT_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --port */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ optionNumericVal,
     /* desc, NAME, name */ PORT_DESC, PORT_NAME, PORT_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 15, VALUE_OPT_CA,
     /* equiv idx, value */ 15, VALUE_OPT_CA,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ CA_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --ca */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ CA_DESC, CA_NAME, CA_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 16, VALUE_OPT_X509,
     /* equiv idx, value */ 16, VALUE_OPT_X509,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ X509_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --x509 */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ X509_DESC, X509_NAME, X509_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ 17, VALUE_OPT_LOCAL,
     /* equiv idx, value */ 17, VALUE_OPT_LOCAL,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ LOCAL_FLAGS, 0,
     /* last opt argumnt */ { NULL }, /* --local */
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ NULL,
     /* desc, NAME, name */ LOCAL_DESC, LOCAL_NAME, LOCAL_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ INDEX_OPT_VERSION, VALUE_OPT_VERSION,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_VERSION,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ VER_FLAGS, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ VER_PROC,
     /* desc, NAME, name */ VER_DESC, NULL, VER_name,
     /* disablement strs */ NULL, NULL },



  {  /* entry idx, value */ INDEX_OPT_HELP, VALUE_OPT_HELP,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_HELP,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ OPTST_IMM | OPTST_NO_INIT, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL, NULL,
     /* option proc      */ doUsageOpt,
     /* desc, NAME, name */ HELP_DESC, NULL, HELP_name,
     /* disablement strs */ NULL, NULL },

  {  /* entry idx, value */ INDEX_OPT_MORE_HELP, VALUE_OPT_MORE_HELP,
     /* equiv idx value  */ NO_EQUIVALENT, VALUE_OPT_MORE_HELP,
     /* equivalenced to  */ NO_EQUIVALENT,
     /* min, max, act ct */ 0, 1, 0,
     /* opt state flags  */ MORE_HELP_FLAGS, 0,
     /* last opt argumnt */ { NULL },
     /* arg list/cookie  */ NULL,
     /* must/cannot opts */ NULL,  NULL,
     /* option proc      */ optionPagedUsage,
     /* desc, NAME, name */ MORE_HELP_DESC, NULL, MORE_HELP_name,
     /* disablement strs */ NULL, NULL }
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Define the danetool Option Environment
 */
#define zPROGNAME       (danetool_opt_strs+1989)
#define zUsageTitle     (danetool_opt_strs+1998)
#define zRcName         NULL
#define apzHomeList     NULL
#define zBugsAddr       (danetool_opt_strs+2101)
#define zExplain        (danetool_opt_strs+2120)
#define zDetail         (danetool_opt_strs+2123)
#define zFullVersion    (danetool_opt_strs+2183)
/* extracted from optcode.tlib near line 350 */

#if defined(ENABLE_NLS)
# define OPTPROC_BASE OPTPROC_TRANSLATE | OPTPROC_NXLAT_OPT
  static tOptionXlateProc translate_option_strings;
#else
# define OPTPROC_BASE OPTPROC_NONE
# define translate_option_strings NULL
#endif /* ENABLE_NLS */


#define danetool_full_usage (NULL)

#define danetool_short_usage (danetool_opt_strs+2202)

#endif /* not defined __doxygen__ */

/*
 *  Create the static procedure(s) declared above.
 */
/**
 * The callout function that invokes the optionUsage function.
 *
 * @param pOptions the AutoOpts option description structure
 * @param pOptDesc the descriptor for the "help" (usage) option.
 * @noreturn
 */
static void
doUsageOpt(tOptions * pOptions, tOptDesc * pOptDesc)
{
    optionUsage(&danetoolOptions, DANETOOL_EXIT_SUCCESS);
    /* NOTREACHED */
    (void)pOptDesc;
    (void)pOptions;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the debug option.
 *
 * @param pOptions the danetool options data structure
 * @param pOptDesc the option descriptor for this option.
 */
static void
doOptDebug(tOptions* pOptions, tOptDesc* pOptDesc)
{
    static struct {long rmin, rmax;} const rng[1] = {
        { 0 ,  9999 } };
    int  ix;

    if (pOptions <= OPTPROC_EMIT_LIMIT)
        goto emit_ranges;
    optionNumericVal(pOptions, pOptDesc);

    for (ix = 0; ix < 1; ix++) {
        if (pOptDesc->optArg.argInt < rng[ix].rmin)
            continue;  /* ranges need not be ordered. */
        if (pOptDesc->optArg.argInt == rng[ix].rmin)
            return;
        if (rng[ix].rmax == LONG_MIN)
            continue;
        if (pOptDesc->optArg.argInt <= rng[ix].rmax)
            return;
    }

    option_usage_fp = stderr;

emit_ranges:

    optionShowRange(pOptions, pOptDesc, (void *)rng, 1);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the infile option.
 *
 * @param pOptions the danetool options data structure
 * @param pOptDesc the option descriptor for this option.
 */
static void
doOptInfile(tOptions* pOptions, tOptDesc* pOptDesc)
{
    static teOptFileType const  type =
        FTYPE_MODE_MUST_EXIST + FTYPE_MODE_NO_OPEN;
    static tuFileMode           mode;
#ifndef O_CLOEXEC
#  define O_CLOEXEC 0
#endif
    mode.file_flags = O_CLOEXEC;

    optionFileCheck(pOptions, pOptDesc, type, mode);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the inraw option.
 *
 * @param pOptions the danetool options data structure
 * @param pOptDesc the option descriptor for this option.
 */
static void
doOptInraw(tOptions* pOptions, tOptDesc* pOptDesc)
{
    int res = optionAlias(pOptions, pOptDesc, INDEX_OPT_INDER);
    if ((res != 0) && ((pOptions->fOptSet & OPTPROC_ERRSTOP) != 0))
        USAGE(DANETOOL_EXIT_FAILURE);

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
 * Code to handle the outraw option.
 *
 * @param pOptions the danetool options data structure
 * @param pOptDesc the option descriptor for this option.
 */
static void
doOptOutraw(tOptions* pOptions, tOptDesc* pOptDesc)
{
    int res = optionAlias(pOptions, pOptDesc, INDEX_OPT_OUTDER);
    if ((res != 0) && ((pOptions->fOptSet & OPTPROC_ERRSTOP) != 0))
        USAGE(DANETOOL_EXIT_FAILURE);

}
/* extracted from optmain.tlib near line 1113 */

/**
 * The directory containing the data associated with danetool.
 */
#ifndef  PKGDATADIR
# define PKGDATADIR ""
#endif

/**
 * Information about the person or institution that packaged danetool
 * for the current distribution.
 */
#ifndef  WITH_PACKAGER
# define danetool_packager_info NULL
#else
static char const danetool_packager_info[] =
    "Packaged by " WITH_PACKAGER

# ifdef WITH_PACKAGER_VERSION
        " ("WITH_PACKAGER_VERSION")"
# endif

# ifdef WITH_PACKAGER_BUG_REPORTS
    "\nReport danetool bugs to " WITH_PACKAGER_BUG_REPORTS
# endif
    "\n";
#endif
#ifndef __doxygen__

#endif /* __doxygen__ */
/**
 * The option definitions for danetool.  The one structure that
 * binds them all.
 */
tOptions danetoolOptions = {
    OPTIONS_STRUCT_VERSION,
    0, NULL,                    /* original argc + argv    */
    ( OPTPROC_BASE
    + OPTPROC_ERRSTOP
    + OPTPROC_SHORTOPT
    + OPTPROC_LONGOPT
    + OPTPROC_NO_REQ_OPT
    + OPTPROC_NEGATIONS
    + OPTPROC_NO_ARGS
    + OPTPROC_GNUUSAGE
    + OPTPROC_MISUSE ),
    0, NULL,                    /* current option index, current option */
    NULL,         NULL,         zPROGNAME,
    zRcName,      zCopyright,   zLicenseDescrip,
    zFullVersion, apzHomeList,  zUsageTitle,
    zExplain,     zDetail,      optDesc,
    zBugsAddr,                  /* address to send bugs to */
    NULL, NULL,                 /* extensions/saved state  */
    optionUsage, /* usage procedure */
    translate_option_strings,   /* translation procedure */
    /*
     *  Indexes to special options
     */
    { INDEX_OPT_MORE_HELP, /* more-help option index */
      NO_EQUIVALENT, /* save option index */
      NO_EQUIVALENT, /* '-#' option index */
      NO_EQUIVALENT /* index of default opt */
    },
    21 /* full option count */, 18 /* user option count */,
    danetool_full_usage, danetool_short_usage,
    NULL, NULL,
    PKGDATADIR, danetool_packager_info
};

#if ENABLE_NLS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <autoopts/usage-txt.h>

static char* AO_gettext(char const* pz);
static void  coerce_it(void** s);

/**
 * AutoGen specific wrapper function for gettext.
 * It relies on the macro _() to convert from English to the target
 * language, then strdup-duplicates the result string.
 *
 * @param[in] pz the input text used as a lookup key.
 * @returns the translated text (if there is one),
 *   or the original text (if not).
 */
static char *
AO_gettext(char const* pz)
{
    char* pzRes;
    if (pz == NULL)
        return NULL;
    pzRes = _(pz);
    if (pzRes == pz)
        return pzRes;
    pzRes = strdup(pzRes);
    if (pzRes == NULL) {
        fputs(_("No memory for duping translated strings\n"), stderr);
        exit(DANETOOL_EXIT_FAILURE);
    }
    return pzRes;
}

static void coerce_it(void** s) { *s = AO_gettext(*s);
}

/**
 * Translate all the translatable strings in the danetoolOptions
 * structure defined above.  This is done only once.
 */
static void
translate_option_strings(void)
{
    tOptions * const pOpt = &danetoolOptions;

    /*
     *  Guard against re-translation.  It won't work.  The strings will have
     *  been changed by the first pass through this code.  One shot only.
     */
    if (option_usage_text.field_ct != 0) {
        /*
         *  Do the translations.  The first pointer follows the field count
         *  field.  The field count field is the size of a pointer.
         */
        tOptDesc * pOD = pOpt->pOptDesc;
        char **    ppz = (char**)(void*)&(option_usage_text);
        int        ix  = option_usage_text.field_ct;

        do {
            ppz++;
            *ppz = AO_gettext(*ppz);
        } while (--ix > 0);

        coerce_it((void*)&(pOpt->pzCopyright));
        coerce_it((void*)&(pOpt->pzCopyNotice));
        coerce_it((void*)&(pOpt->pzFullVersion));
        coerce_it((void*)&(pOpt->pzUsageTitle));
        coerce_it((void*)&(pOpt->pzExplain));
        coerce_it((void*)&(pOpt->pzDetail));
        coerce_it((void*)&(pOpt->pzPackager));
        coerce_it((void*)&(pOpt->pzShortUsage));
        option_usage_text.field_ct = 0;

        for (ix = pOpt->optCt; ix > 0; ix--, pOD++)
            coerce_it((void*)&(pOD->pzText));
    }

    if ((pOpt->fOptSet & OPTPROC_NXLAT_OPT_CFG) == 0) {
        tOptDesc * pOD = pOpt->pOptDesc;
        int        ix;

        for (ix = pOpt->optCt; ix > 0; ix--, pOD++) {
            coerce_it((void*)&(pOD->pz_Name));
            coerce_it((void*)&(pOD->pz_DisableName));
            coerce_it((void*)&(pOD->pz_DisablePfx));
        }
        /* prevent re-translation */
        danetoolOptions.fOptSet |= OPTPROC_NXLAT_OPT_CFG | OPTPROC_NXLAT_OPT;
    }
}

#endif /* ENABLE_NLS */

#ifdef  __cplusplus
}
#endif
/* danetool-args.c ends here */
