/*
 * Copyright (c) 2012-2019 Belledonne Communications SARL.
 *
 * This file is part of belle-sip.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef belle_sip_stack_h
#define belle_sip_stack_h

#include "belle-sip/mainloop.h"
#include "belle-sip/types.h"

struct belle_sip_timer_config {
	int T1;
	int T2;
	int T3;
	int T4;
};
typedef struct belle_sip_timer_config belle_sip_timer_config_t;

typedef struct belle_sip_digest_authentication_policy belle_sip_digest_authentication_policy_t;

BELLE_SIP_BEGIN_DECLS

BELLESIP_EXPORT belle_sip_digest_authentication_policy_t *belle_sip_digest_authentication_policy_new(void);

BELLESIP_EXPORT unsigned char
belle_sip_digest_authentication_policy_get_allow_md5(const belle_sip_digest_authentication_policy_t *obj);

BELLESIP_EXPORT void belle_sip_digest_authentication_policy_set_allow_md5(belle_sip_digest_authentication_policy_t *obj,
                                                                          unsigned char value);

BELLESIP_EXPORT unsigned char
belle_sip_digest_authentication_policy_get_allow_no_qop(const belle_sip_digest_authentication_policy_t *obj);

BELLESIP_EXPORT void
belle_sip_digest_authentication_policy_set_allow_no_qop(belle_sip_digest_authentication_policy_t *obj,
                                                        unsigned char value);

/**
 * This only affects the resolution of SIP URI in absence of port number, and in absence of SRV record for the SIP
 *domain. The well known ports (udp/tcp and tls/dtls) are static.
 * @param well known port value
 **/
BELLESIP_EXPORT void belle_sip_stack_set_well_known_port(int value);

BELLESIP_EXPORT void belle_sip_stack_set_well_known_port_tls(int value);

BELLESIP_EXPORT int belle_sip_stack_get_well_known_port(void);

BELLESIP_EXPORT int belle_sip_stack_get_well_known_port_tls(void);

BELLESIP_EXPORT belle_sip_stack_t *belle_sip_stack_new(const char *properties);

BELLESIP_EXPORT belle_sip_listening_point_t *
belle_sip_stack_create_listening_point(belle_sip_stack_t *s, const char *ipaddress, int port, const char *transport);

BELLESIP_EXPORT void belle_sip_stack_delete_listening_point(belle_sip_stack_t *s, belle_sip_listening_point_t *lp);

BELLESIP_EXPORT belle_sip_provider_t *belle_sip_stack_create_provider(belle_sip_stack_t *s,
                                                                      belle_sip_listening_point_t *lp);

BELLESIP_EXPORT belle_http_provider_t *belle_sip_stack_create_http_provider(belle_sip_stack_t *s, const char *bind_ip);

#define BELLE_SIP_HTTP_TRANSPORT_TCP 0x01
#define BELLE_SIP_HTTP_TRANSPORT_TLS 0x02
/**
 * Create a HTTP provider attached to a given stack enabling specific transports
 * @param[in]	s	The stack to attach the transport
 * @param[in]	bind_ip
 * @param[in]	transports a mask of authorized transports for this provider, availables are
 * BELLE_SIP_HTTP_TRANSPORT_TCP and BELLE_SIP_HTTP_TRANSPORT_TLS
 *
 * @return The HTTP provider
 *
 */
BELLESIP_EXPORT belle_http_provider_t *belle_sip_stack_create_http_provider_with_transports(belle_sip_stack_t *s,
                                                                                            const char *bind_ip,
                                                                                            const uint8_t transports);

BELLESIP_EXPORT belle_sip_main_loop_t *belle_sip_stack_get_main_loop(belle_sip_stack_t *stack);

BELLESIP_EXPORT void belle_sip_stack_main(belle_sip_stack_t *stack);

BELLESIP_EXPORT void belle_sip_stack_sleep(belle_sip_stack_t *stack, unsigned int milliseconds);

/*the transport timeout is typically the maximum time given for making a connection*/
BELLESIP_EXPORT void belle_sip_stack_set_transport_timeout(belle_sip_stack_t *stack, int timeout_ms);

BELLESIP_EXPORT int belle_sip_stack_get_transport_timeout(const belle_sip_stack_t *stack);

BELLESIP_EXPORT int belle_sip_stack_get_dns_timeout(const belle_sip_stack_t *stack);

BELLESIP_EXPORT void belle_sip_stack_set_dns_timeout(belle_sip_stack_t *stack, int timeout);

BELLESIP_EXPORT unsigned char belle_sip_stack_dns_srv_enabled(const belle_sip_stack_t *stack);

BELLESIP_EXPORT void belle_sip_stack_enable_dns_srv(belle_sip_stack_t *stack, unsigned char enable);

BELLESIP_EXPORT unsigned char belle_sip_stack_dns_search_enabled(const belle_sip_stack_t *stack);

BELLESIP_EXPORT void belle_sip_stack_enable_dns_search(belle_sip_stack_t *stack, unsigned char enable);

/**
 * Override system's DNS servers used for DNS resolving by app-supplied list of dns servers.
 * @param stack the stack
 * @param servers a list of char*. It is copied internally.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_dns_servers(belle_sip_stack_t *stack, const belle_sip_list_t *servers);

/**
 * Get the additional DNS hosts file.
 * @return The path to the additional DNS hosts file.
 **/
BELLESIP_EXPORT const char *belle_sip_stack_get_dns_user_hosts_file(const belle_sip_stack_t *stack);

/**
 * Can be used to load an additional DNS hosts file for tests.
 * @param stack
 * @param hosts_file The path to the additional DNS hosts file to load.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_dns_user_hosts_file(belle_sip_stack_t *stack, const char *hosts_file);

/**
 * Get the overriding DNS resolv.conf file.
 * @return The path to the overriding DNS resolv.conf file.
 **/
BELLESIP_EXPORT const char *belle_sip_stack_get_dns_resolv_conf_file(const belle_sip_stack_t *stack);

/**
 * Can be used to load an overriding DNS resolv.conf file for tests.
 * @param stack
 * @param hosts_file The path to the overriding DNS resolv.conf file to load.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_dns_resolv_conf_file(belle_sip_stack_t *stack, const char *hosts_file);

/**
 * Returns the time interval in seconds after which a connection must be closed when inactive.
 **/
BELLESIP_EXPORT int belle_sip_stack_get_inactive_transport_timeout(const belle_sip_stack_t *stack);

/**
 * Sets the time interval in seconds after which a connection must be closed when inactive.
 * By inactive, it is meant that the connection hasn't been used to send or recv data.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_inactive_transport_timeout(belle_sip_stack_t *stack, int seconds);

/**
 * Set the time interval in seconds after which a connection is considered to be unreliable because
 * no data was received over it.
 * The special value 0 means that a connection will never be considered as unreliable.
 * See belle_sip_provider_clean_unreliable_channels().
 */
BELLESIP_EXPORT void belle_sip_stack_set_unreliable_connection_timeout(belle_sip_stack_t *stack, int seconds);

/**
 * Get the time interval in seconds after which a connection is considered to be unreliable because
 * no data was received over it.
 * See belle_sip_provider_clean_unreliable_channels().
 */
BELLESIP_EXPORT int belle_sip_stack_get_unreliable_connection_timeout(const belle_sip_stack_t *stack);

/**
 * Set the default dscp value to be used for all SIP sockets created and used in the stack.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_default_dscp(belle_sip_stack_t *stack, int dscp);

/**
 * Get the default dscp value to be used for all SIP sockets created and used in the stack.
 **/
BELLESIP_EXPORT int belle_sip_stack_get_default_dscp(belle_sip_stack_t *stack);

/**
 * Returns TRUE if TLS support has been compiled into, FALSE otherwise.
 **/
BELLESIP_EXPORT int belle_sip_stack_tls_available(belle_sip_stack_t *stack);

/**
 * Returns TRUE if the content encoding support has been compiled in, FALSE otherwise.
 **/
BELLESIP_EXPORT int belle_sip_stack_content_encoding_available(belle_sip_stack_t *stack, const char *content_encoding);

/*
 * returns timer config for this stack
 **/
BELLESIP_EXPORT const belle_sip_timer_config_t *belle_sip_stack_get_timer_config(const belle_sip_stack_t *stack);

/*
 *
 * set sip timer config to be used for this stack
 **/
BELLESIP_EXPORT void belle_sip_stack_set_timer_config(belle_sip_stack_t *stack,
                                                      const belle_sip_timer_config_t *timer_config);

BELLESIP_EXPORT void belle_sip_stack_set_http_proxy_host(belle_sip_stack_t *stack, const char *proxy_addr);
BELLESIP_EXPORT void belle_sip_stack_set_http_proxy_port(belle_sip_stack_t *stack, int port);
BELLESIP_EXPORT const char *belle_sip_stack_get_http_proxy_host(const belle_sip_stack_t *stack);
BELLESIP_EXPORT int belle_sip_stack_get_http_proxy_port(const belle_sip_stack_t *stack);

/**
 * Enable the reconnection to the primary server when it is up again as soon as possible.
 * When activated, instead of closing if the current server is not in the refreshed priority ordered servers' list
 * the bellesip channel will close if the current server is not the first server of this list.
 * As a result, it will try to reconnect to the server with the highest priority if it is not the case when records
 * are expiring.
 * @param prov
 * @param enabled 0 to disable
 **/
BELLESIP_EXPORT void belle_sip_stack_enable_reconnect_to_primary_asap(belle_sip_stack_t *stack, int enabled);

/**
 * Returns if the reconnection to the primary server is enabled.
 * @param prov
 * @see belle_sip_provider_enable_reconnect_to_primary_asap()
 **/
BELLESIP_EXPORT int belle_sip_stack_reconnect_to_primary_asap_enabled(const belle_sip_stack_t *stack);

/**
 * Configure security policies for digest authentication.
 */
BELLESIP_EXPORT void belle_sip_stack_set_digest_authentication_policy(belle_sip_stack_t *stack,
                                                                      belle_sip_digest_authentication_policy_t *policy);

BELLESIP_EXPORT const belle_sip_digest_authentication_policy_t *
belle_sip_stack_get_digest_authentication_policy(const belle_sip_stack_t *stack);

/*
 * The following functions are for testing (non regression tests) ONLY
 */

/**
 * Can be used to simulate network transmission delays, for tests.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_tx_delay(belle_sip_stack_t *stack, int delay_ms);
/**
 * Can be used to simulate network sending error, for tests.
 * @param stack
 * @param send_error if <0, will cause channel error to be reported
 **/

BELLESIP_EXPORT void belle_sip_stack_set_send_error(belle_sip_stack_t *stack, int send_error);

/**
 * Can be used to simulate network transmission delays, for tests.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_resolver_tx_delay(belle_sip_stack_t *stack, int delay_ms);

/**
 * Can be used to simulate network sending error, for tests.
 * @param stack
 * @param send_error if <0, will cause the resolver to fail with this error code.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_resolver_send_error(belle_sip_stack_t *stack, int send_error);

/**
 * Requests TCP/TLS client connection to bind a on specific port. This is for test ONLY.
 **/
BELLESIP_EXPORT void belle_sip_stack_set_client_bind_port(belle_sip_stack_t *stack, int port);

#define BELLE_SIP_DNS_APPLE_DNS_SERVICE 0x01
#define BELLE_SIP_DNS_DNS_C 0x02
/**
 * Set the dns engine to use.
 * @param dns_engine	Must be one of BELLE_SIP_DNS_APPLE_DNS_SERVICE or BELLE_SIP_DNS_DNS_C
 **/
BELLESIP_EXPORT void belle_sip_stack_set_dns_engine(belle_sip_stack_t *stack, unsigned char dns_engine);
/**
 * Get the dns engine used
 * @return BELLE_SIP_DNS_APPLE_DNS_SERVICE or BELLE_SIP_DNS_DNS_C
 **/
BELLESIP_EXPORT unsigned char belle_sip_stack_get_dns_engine(const belle_sip_stack_t *stack);

/**
 * Requests the stack to simulate a router that doesn't respond to SRV requests. This is for test ONLY.
 **/
BELLESIP_EXPORT void belle_sip_stack_simulate_non_working_srv(belle_sip_stack_t *stack, int yesno);

/*
 * End of test functions.
 */

BELLE_SIP_END_DECLS

#endif
