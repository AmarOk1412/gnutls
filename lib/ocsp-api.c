/*
 * Copyright (C) 2012-2017 Free Software Foundation, Inc.
 * Copyright (C) 2017 Red Hat, Inc.
 *
 * Author: Simon Josefsson, Nikos Mavrogiannopoulos
 *
 * This file is part of GnuTLS.
 *
 * The GnuTLS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 */

/*
 *  Status Request (OCSP) API.
 */

#include "gnutls_int.h"
#include "errors.h"
#include <auth.h>
#include <auth/cert.h>
#include <handshake.h>

#ifdef ENABLE_OCSP


/**
 * gnutls_ocsp_status_request_get:
 * @session: is a #gnutls_session_t type.
 * @response: a #gnutls_datum_t with DER encoded OCSP response
 *
 * This function returns the OCSP status response received
 * from the TLS server. The @response should be treated as
 * constant. If no OCSP response is available then
 * %GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE is returned.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.1.3
 **/
int
gnutls_ocsp_status_request_get(gnutls_session_t session,
			       gnutls_datum_t * response)
{
	return gnutls_ocsp_status_request_get2(session, 0, response);
}

/**
 * gnutls_ocsp_status_request_get2:
 * @session: is a #gnutls_session_t type.
 * @idx: the index of peer's certificate
 * @response: a #gnutls_datum_t with DER encoded OCSP response
 *
 * This function returns the OCSP status response received
 * from the TLS server for the certificate index provided.
 * The index corresponds to certificates as returned by
 * gnutls_certificate_get_peers. When index is zero this
 * function operates identically to gnutls_ocsp_status_request_get().
 *
 * The returned @response should be treated as
 * constant. If no OCSP response is available for the
 * given index then %GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE
 * is returned.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.6.xx
 **/
int
gnutls_ocsp_status_request_get2(gnutls_session_t session,
			        unsigned idx,
			        gnutls_datum_t * response)
{
	const version_entry_st *ver = get_version(session);
	cert_auth_info_t info = _gnutls_get_auth_info(session, GNUTLS_CRD_CERTIFICATE);

	if (!ver->tls13_sem && session->security_parameters.entity == GNUTLS_SERVER)
		return gnutls_assert_val(GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE);

	if (info == NULL || info->raw_ocsp_list == NULL ||
	    info->nocsp <= idx || info->raw_ocsp_list[idx].size == 0)
		return
		    gnutls_assert_val
		    (GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE);

	response->data = info->raw_ocsp_list[idx].data;
	response->size = info->raw_ocsp_list[idx].size;

	return 0;
}

static int
legacy_ocsp_func_emu(gnutls_session_t session, const gnutls_cert_info_st *cinfo,
		     void *ptr, gnutls_datum_t * ocsp_response)
{
	legacy_ocsp_func_st *s = ptr;

	if (cinfo->cert_index == 0)
		return s->func(session, s->ptr, ocsp_response);

	return GNUTLS_E_NO_CERTIFICATE_STATUS;
}

/**
 * gnutls_certificate_set_ocsp_status_request_function:
 * @sc: is a #gnutls_certificate_credentials_t type.
 * @ocsp_func: function pointer to OCSP status request callback.
 * @ptr: opaque pointer passed to callback function
 *
 * This function is to be used by server to register a callback to
 * handle OCSP status requests from the client.  The callback will be
 * invoked if the client supplied a status-request OCSP extension.
 * The callback function prototype is:
 *
 * typedef int (*gnutls_status_request_ocsp_func)
 *    (gnutls_session_t session, void *ptr, gnutls_datum_t *ocsp_response);
 *
 * The callback will be invoked if the client requests an OCSP certificate
 * status.  The callback may return %GNUTLS_E_NO_CERTIFICATE_STATUS, if
 * there is no recent OCSP response. If the callback returns %GNUTLS_E_SUCCESS,
 * it is expected to have the @ocsp_response field set with a valid (DER-encoded)
 * OCSP response. The response must be a value allocated using gnutls_malloc(),
 * and will be deinitialized by the caller.
 *
 * It is possible to set a specific callback for each provided certificate
 * using gnutls_certificate_set_ocsp_status_request_function3().
 *
 * Since: 3.1.3
 **/
void
gnutls_certificate_set_ocsp_status_request_function(gnutls_certificate_credentials_t sc,
						    gnutls_status_request_ocsp_func ocsp_func,
						    void *ptr)
{
	sc->glob_ocsp_func = legacy_ocsp_func_emu;
	sc->glob_ocsp_func_ptr = &sc->glob_legacy_ocsp;

	sc->glob_legacy_ocsp.func = ocsp_func;
	sc->glob_legacy_ocsp.ptr = ptr;
}

/**
 * gnutls_certificate_set_ocsp_status_request_function2:
 * @sc: is a #gnutls_certificate_credentials_t type.
 * @idx: is a certificate index as returned by gnutls_certificate_set_key() and friends
 * @ocsp_func: function pointer to OCSP status request callback.
 * @ptr: opaque pointer passed to callback function
 *
 * This function is to be used by server to register a callback to
 * provide OCSP status requests that correspond to the indexed certificate chain
 * from the client.  The callback will be invoked if the client supplied a 
 * status-request OCSP extension.
 *
 * The callback function prototype is:
 *
 * typedef int (*gnutls_status_request_ocsp_func)
 *    (gnutls_session_t session, void *ptr, gnutls_datum_t *ocsp_response);
 *
 * The callback will be invoked if the client requests an OCSP certificate
 * status.  The callback may return %GNUTLS_E_NO_CERTIFICATE_STATUS, if
 * there is no recent OCSP response. If the callback returns %GNUTLS_E_SUCCESS,
 * it is expected to have the @ocsp_response field set with a valid (DER-encoded)
 * OCSP response. The response must be a value allocated using gnutls_malloc(),
 * and will be deinitialized by the caller.
 *
 * Note: the ability to set multiple OCSP responses per credential
 * structure via the index @idx was added in version 3.5.6. To keep
 * backwards compatibility, it requires using gnutls_certificate_set_flags()
 * with the %GNUTLS_CERTIFICATE_API_V2 flag to make the set certificate
 * functions return an index usable by this function.
 *
 * This function works with the pre-loaded certificate chains, and
 * must be called after they are set. When the certificate chains are
 * obtained via a callback, i.e., when gnutls_certificate_set_retrieve_function()
 * and friends are used, use gnutls_certificate_set_ocsp_status_request_function3()
 * with %GNUTLS_OCSP_CB_GLOBAL_SET flag instead.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.5.5
 **/
int
gnutls_certificate_set_ocsp_status_request_function2(gnutls_certificate_credentials_t sc,
						     unsigned idx,
						     gnutls_status_request_ocsp_func ocsp_func,
						     void *ptr)
{
	if (idx >= sc->ncerts)
		return gnutls_assert_val(GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE);

	sc->certs[idx].legacy_ocsp.func = ocsp_func;
	sc->certs[idx].legacy_ocsp.ptr = ptr;

	return gnutls_certificate_set_ocsp_status_request_function3(sc, idx,
								    legacy_ocsp_func_emu,
								    &sc->certs[idx].legacy_ocsp,
								    0);
}

/**
 * gnutls_certificate_set_ocsp_status_request_function3:
 * @sc: is a #gnutls_certificate_credentials_t type.
 * @idx: is a certificate index as returned by gnutls_certificate_set_key() and friends
 * @ocsp_func: function pointer to OCSP status request callback.
 * @ptr: opaque pointer passed to callback function
 * @flags: must be zero
 *
 * This function is to be used by server to register a callback to
 * provide OCSP status requests that correspond to the indexed certificate chain
 * from the client.  The callback will be invoked if the client supplied a 
 * status-request OCSP extension.
 *
 * The callback function prototype is:
 *
 * typedef int (*gnutls_status_request_ocsp_func2)
 *    (gnutls_session_t session, const gnutls_cert_info_st *cinfo, void *ptr, gnutls_datum_t *ocsp_resp);
 *
 * The callback will be invoked if the client requests an OCSP certificate
 * status.  The callback may return %GNUTLS_E_NO_CERTIFICATE_STATUS, if
 * there is no recent OCSP response. If the callback returns %GNUTLS_E_SUCCESS,
 * it is expected to have the @ocsp_response field set with a valid (DER-encoded)
 * OCSP response. The response must be a value allocated using gnutls_malloc(),
 * and will be deinitialized by the caller.
 *
 * This function allows a server to provide more than a single OCSP responses
 * corresponding to each certificate in the certificate chain.
 *
 * When the flag %GNUTLS_OCSP_CB_GLOBAL_SET is specified in @flags, this
 * function can be used to set a callback that is used even when the
 * certificates are provided by the application via a callback. That is,
 * when gnutls_certificate_set_retrieve_function() and friends are used.
 * In that case the callback will be called with the selected certificate.
 *
 * Note: the ability to set multiple OCSP responses per credential
 * structure via the index @idx was added in version 3.5.6. To keep
 * backwards compatibility, it requires using gnutls_certificate_set_flags()
 * with the %GNUTLS_CERTIFICATE_API_V2 flag to make the set certificate
 * functions return an index usable by this function.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.6.xx
 **/
int
gnutls_certificate_set_ocsp_status_request_function3(gnutls_certificate_credentials_t sc,
						     unsigned idx,
						     gnutls_status_request_ocsp_func2 ocsp_func,
						     void *ptr,
						     unsigned flags)
{
	if (flags & GNUTLS_OCSP_CB_GLOBAL_SET) {
		sc->glob_ocsp_func = ocsp_func;
		sc->glob_ocsp_func_ptr = ptr;

		return 0;
	}

	if (idx >= sc->ncerts)
		return gnutls_assert_val(GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE);

	sc->certs[idx].ocsp_func = ocsp_func;
	sc->certs[idx].ocsp_func_ptr = ptr;

	return 0;
}

static int file_ocsp_func(gnutls_session_t session,
			  const gnutls_cert_info_st *cinfo,
			  void *ptr,
			  gnutls_datum_t * ocsp_response)
{
	int ret;
	const char *file = ptr;

	if (cinfo->cert_index > 0)
		return GNUTLS_E_NO_CERTIFICATE_STATUS;

	ret = gnutls_load_file(file, ocsp_response);
	if (ret < 0)
		return gnutls_assert_val(GNUTLS_E_NO_CERTIFICATE_STATUS);

	return 0;
}

/**
 * gnutls_certificate_set_ocsp_status_request_file:
 * @sc: is a credentials structure.
 * @response_file: a filename of the OCSP response
 * @idx: is a certificate index as returned by gnutls_certificate_set_key() and friends
 *
 * This function sets the filename of an OCSP response, that will be
 * sent to the client if requests an OCSP certificate status for
 * the certificate chain specified by @idx.
 *
 * This is a convenience function which may be inefficient on busy servers since
 * the file is opened on every access. Use 
 * gnutls_certificate_set_ocsp_status_request_function2() to fine-tune
 * file accesses.
 *
 * Note: the ability to set multiple OCSP responses per credential
 * structure via the index @idx was added in version 3.5.6. To keep
 * backwards compatibility, it requires using gnutls_certificate_set_flags()
 * with the %GNUTLS_CERTIFICATE_API_V2 flag to make the set certificate
 * functions return an index usable by this function.
 *
 * This function must be called after setting any certificates, and
 * cannot be used for certificates that are provided via a callback --
 * that is when gnutls_certificate_set_retrieve_function() is used.
 *
 * Returns: On success, %GNUTLS_E_SUCCESS (0) is returned,
 *   otherwise a negative error code is returned.
 *
 * Since: 3.1.3
 **/
int
gnutls_certificate_set_ocsp_status_request_file(gnutls_certificate_credentials_t sc,
						const char *response_file,
						unsigned idx)
{
	if (idx >= sc->ncerts)
		return gnutls_assert_val(GNUTLS_E_REQUESTED_DATA_NOT_AVAILABLE);

	gnutls_free(sc->certs[idx].ocsp_response_file);
	sc->certs[idx].ocsp_response_file = gnutls_strdup(response_file);
	if (sc->certs[idx].ocsp_response_file == NULL)
		return gnutls_assert_val(GNUTLS_E_MEMORY_ERROR);

	return gnutls_certificate_set_ocsp_status_request_function3(sc, idx, file_ocsp_func, sc->certs[idx].ocsp_response_file, 0);
}

/**
 * gnutls_ocsp_status_request_is_checked:
 * @session: is a gnutls session
 * @flags: should be zero or %GNUTLS_OCSP_SR_IS_AVAIL
 *
 * When flags are zero this function returns non-zero if a valid OCSP status
 * response was included in the TLS handshake. That is, an OCSP status response
 * which is not too old or superseded. It returns zero otherwise.
 *
 * When the flag %GNUTLS_OCSP_SR_IS_AVAIL is specified, the function
 * returns non-zero if an OCSP status response was included in the handshake
 * even if it was invalid. Otherwise, if no OCSP status response was included,
 * it returns zero. The %GNUTLS_OCSP_SR_IS_AVAIL flag was introduced in GnuTLS 3.4.0.
 *
 * This is a helper function when needing to decide whether to perform an
 * explicit OCSP validity check on the peer's certificate. Should be called after
 * any of gnutls_certificate_verify_peers*() are called.
 *
 * Returns: non zero if the response was valid, or a zero if it wasn't sent,
 * or sent and was invalid.
 *
 * Since: 3.1.4
 **/
int
gnutls_ocsp_status_request_is_checked(gnutls_session_t session,
				      unsigned int flags)
{
	int ret;
	gnutls_datum_t data;

	if (flags & GNUTLS_OCSP_SR_IS_AVAIL) {
		ret = gnutls_ocsp_status_request_get(session, &data);
		if (ret < 0)
			return gnutls_assert_val(0);

		if (data.data == NULL)
			return gnutls_assert_val(0);
		return 1;
	}
	return session->internals.ocsp_check_ok;
}

#endif
