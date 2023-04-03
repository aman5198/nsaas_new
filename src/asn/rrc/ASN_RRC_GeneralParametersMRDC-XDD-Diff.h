/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "asn/nr-rrc-15.6.0.asn1"
 * 	`asn1c -fcompound-names -pdu=all -findirect-choice -fno-include-deps -gen-PER -no-gen-OER -no-gen-example -D rrc`
 */

#ifndef	_ASN_RRC_GeneralParametersMRDC_XDD_Diff_H_
#define	_ASN_RRC_GeneralParametersMRDC_XDD_Diff_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ASN_RRC_GeneralParametersMRDC_XDD_Diff__splitSRB_WithOneUL_Path {
	ASN_RRC_GeneralParametersMRDC_XDD_Diff__splitSRB_WithOneUL_Path_supported	= 0
} e_ASN_RRC_GeneralParametersMRDC_XDD_Diff__splitSRB_WithOneUL_Path;
typedef enum ASN_RRC_GeneralParametersMRDC_XDD_Diff__splitDRB_withUL_Both_MCG_SCG {
	ASN_RRC_GeneralParametersMRDC_XDD_Diff__splitDRB_withUL_Both_MCG_SCG_supported	= 0
} e_ASN_RRC_GeneralParametersMRDC_XDD_Diff__splitDRB_withUL_Both_MCG_SCG;
typedef enum ASN_RRC_GeneralParametersMRDC_XDD_Diff__srb3 {
	ASN_RRC_GeneralParametersMRDC_XDD_Diff__srb3_supported	= 0
} e_ASN_RRC_GeneralParametersMRDC_XDD_Diff__srb3;
typedef enum ASN_RRC_GeneralParametersMRDC_XDD_Diff__v2x_EUTRA {
	ASN_RRC_GeneralParametersMRDC_XDD_Diff__v2x_EUTRA_supported	= 0
} e_ASN_RRC_GeneralParametersMRDC_XDD_Diff__v2x_EUTRA;

/* ASN_RRC_GeneralParametersMRDC-XDD-Diff */
typedef struct ASN_RRC_GeneralParametersMRDC_XDD_Diff {
	long	*splitSRB_WithOneUL_Path;	/* OPTIONAL */
	long	*splitDRB_withUL_Both_MCG_SCG;	/* OPTIONAL */
	long	*srb3;	/* OPTIONAL */
	long	*v2x_EUTRA;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ASN_RRC_GeneralParametersMRDC_XDD_Diff_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_splitSRB_WithOneUL_Path_2;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_splitDRB_withUL_Both_MCG_SCG_4;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_srb3_6;	// (Use -fall-defs-global to expose) */
/* extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_v2x_EUTRA_8;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_GeneralParametersMRDC_XDD_Diff;
extern asn_SEQUENCE_specifics_t asn_SPC_ASN_RRC_GeneralParametersMRDC_XDD_Diff_specs_1;
extern asn_TYPE_member_t asn_MBR_ASN_RRC_GeneralParametersMRDC_XDD_Diff_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _ASN_RRC_GeneralParametersMRDC_XDD_Diff_H_ */
#include <asn_internal.h>
