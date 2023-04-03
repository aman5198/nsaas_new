/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "asn/nr-rrc-15.6.0.asn1"
 * 	`asn1c -fcompound-names -pdu=all -findirect-choice -fno-include-deps -gen-PER -no-gen-OER -no-gen-example -D rrc`
 */

#ifndef	_ASN_RRC_AggregatedBandwidth_H_
#define	_ASN_RRC_AggregatedBandwidth_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ASN_RRC_AggregatedBandwidth {
	ASN_RRC_AggregatedBandwidth_mhz50	= 0,
	ASN_RRC_AggregatedBandwidth_mhz100	= 1,
	ASN_RRC_AggregatedBandwidth_mhz150	= 2,
	ASN_RRC_AggregatedBandwidth_mhz200	= 3,
	ASN_RRC_AggregatedBandwidth_mhz250	= 4,
	ASN_RRC_AggregatedBandwidth_mhz300	= 5,
	ASN_RRC_AggregatedBandwidth_mhz350	= 6,
	ASN_RRC_AggregatedBandwidth_mhz400	= 7,
	ASN_RRC_AggregatedBandwidth_mhz450	= 8,
	ASN_RRC_AggregatedBandwidth_mhz500	= 9,
	ASN_RRC_AggregatedBandwidth_mhz550	= 10,
	ASN_RRC_AggregatedBandwidth_mhz600	= 11,
	ASN_RRC_AggregatedBandwidth_mhz650	= 12,
	ASN_RRC_AggregatedBandwidth_mhz700	= 13,
	ASN_RRC_AggregatedBandwidth_mhz750	= 14,
	ASN_RRC_AggregatedBandwidth_mhz800	= 15
} e_ASN_RRC_AggregatedBandwidth;

/* ASN_RRC_AggregatedBandwidth */
typedef long	 ASN_RRC_AggregatedBandwidth_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ASN_RRC_AggregatedBandwidth_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_AggregatedBandwidth;
extern const asn_INTEGER_specifics_t asn_SPC_ASN_RRC_AggregatedBandwidth_specs_1;
asn_struct_free_f ASN_RRC_AggregatedBandwidth_free;
asn_struct_print_f ASN_RRC_AggregatedBandwidth_print;
asn_constr_check_f ASN_RRC_AggregatedBandwidth_constraint;
ber_type_decoder_f ASN_RRC_AggregatedBandwidth_decode_ber;
der_type_encoder_f ASN_RRC_AggregatedBandwidth_encode_der;
xer_type_decoder_f ASN_RRC_AggregatedBandwidth_decode_xer;
xer_type_encoder_f ASN_RRC_AggregatedBandwidth_encode_xer;
per_type_decoder_f ASN_RRC_AggregatedBandwidth_decode_uper;
per_type_encoder_f ASN_RRC_AggregatedBandwidth_encode_uper;
per_type_decoder_f ASN_RRC_AggregatedBandwidth_decode_aper;
per_type_encoder_f ASN_RRC_AggregatedBandwidth_encode_aper;

#ifdef __cplusplus
}
#endif

#endif	/* _ASN_RRC_AggregatedBandwidth_H_ */
#include <asn_internal.h>
