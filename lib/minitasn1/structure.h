
/*************************************************/
/* File: structure.h                             */
/* Description: list of exported object by       */
/*   "structure.c"                               */
/*************************************************/

#ifndef _STRUCTURE_H
#define _STRUCTURE_H


/*************************************/
/* Constants used in asn1_visit_tree */
/*************************************/
#define ASN1_PRINT_NAME             1
#define ASN1_PRINT_NAME_TYPE        2
#define ASN1_PRINT_NAME_TYPE_VALUE  3
#define ASN1_PRINT_ALL              4


asn1_retCode _asn1_create_static_structure(node_asn *pointer,
       char* output_file_name,char *vector_name);

node_asn* _asn1_copy_structure3(node_asn *source_node);

node_asn* _asn1_copy_structure2(node_asn *root,const char *source_name);

node_asn * _asn1_add_node_only(unsigned int type);

node_asn * _asn1_find_left(node_asn *node);

#endif

