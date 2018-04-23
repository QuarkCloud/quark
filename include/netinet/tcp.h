
#ifndef __QKC_NETINET_TCP_H
#define __QKC_NETINET_TCP_H 1

#include <quark_compile.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * User-settable options (used with setsockopt).
 */
#define	TCP_NODELAY	 1	/* Don't delay send to coalesce packets  */
#define	TCP_MAXSEG	 2	/* Set maximum segment size  */
#define TCP_CORK	 3	/* Control sending of partial frames  */
#define TCP_KEEPIDLE	 4	/* Start keeplives after this period */
#define TCP_KEEPINTVL	 5	/* Interval between keepalives */
#define TCP_KEEPCNT	 6	/* Number of keepalives before death */
#define TCP_SYNCNT	 7	/* Number of SYN retransmits */
#define TCP_LINGER2	 8	/* Life time of orphaned FIN-WAIT-2 state */
#define TCP_DEFER_ACCEPT 9	/* Wake up listener only when data arrive */
#define TCP_WINDOW_CLAMP 10	/* Bound advertised window */
#define TCP_INFO	 11	/* Information about this connection. */
#define	TCP_QUICKACK	 12	/* Bock/reenable quick ACKs.  */


enum
{
  TCP_ESTABLISHED = 1,
  TCP_SYN_SENT,
  TCP_SYN_RECV,
  TCP_FIN_WAIT1,
  TCP_FIN_WAIT2,
  TCP_TIME_WAIT,
  TCP_CLOSE,
  TCP_CLOSE_WAIT,
  TCP_LAST_ACK,
  TCP_LISTEN,
  TCP_CLOSING   /* now a valid state */
};

# define TCPOPT_EOL		0
# define TCPOPT_NOP		1
# define TCPOPT_MAXSEG		2
# define TCPOLEN_MAXSEG		4
# define TCPOPT_WINDOW		3
# define TCPOLEN_WINDOW		3
# define TCPOPT_SACK_PERMITTED	4		/* Experimental */
# define TCPOLEN_SACK_PERMITTED	2
# define TCPOPT_SACK		5		/* Experimental */
# define TCPOPT_TIMESTAMP	8
# define TCPOLEN_TIMESTAMP	10
# define TCPOLEN_TSTAMP_APPA	(TCPOLEN_TIMESTAMP+2) /* appendix A */

/*
 * Default maximum segment size for TCP.
 * With an IP MSS of 576, this is 536,
 * but 512 is probably more convenient.
 * This should be defined as MIN(512, IP_MSS - sizeof (struct tcpiphdr)).
 */
# define TCP_MSS	512

# define TCP_MAXWIN	65535	/* largest value for (unscaled) window */

# define TCP_MAX_WINSHIFT	14	/* maximum window shift */

# define SOL_TCP		6	/* TCP level */


# define TCPI_OPT_TIMESTAMPS	1
# define TCPI_OPT_SACK		2
# define TCPI_OPT_WSCALE	4
# define TCPI_OPT_ECN		8

/* Values for tcpi_state.  */
enum tcp_ca_state
{
  TCP_CA_Open = 0,
  TCP_CA_Disorder = 1,
  TCP_CA_CWR = 2,
  TCP_CA_Recovery = 3,
  TCP_CA_Loss = 4
};
/**
struct tcp_info
{
  u_int8_t	tcpi_state;
  u_int8_t	tcpi_ca_state;
  u_int8_t	tcpi_retransmits;
  u_int8_t	tcpi_probes;
  u_int8_t	tcpi_backoff;
  u_int8_t	tcpi_options;
  u_int8_t	tcpi_snd_wscale : 4, tcpi_rcv_wscale : 4;

  u_int32_t	tcpi_rto;
  u_int32_t	tcpi_ato;
  u_int32_t	tcpi_snd_mss;
  u_int32_t	tcpi_rcv_mss;

  u_int32_t	tcpi_unacked;
  u_int32_t	tcpi_sacked;
  u_int32_t	tcpi_lost;
  u_int32_t	tcpi_retrans;
  u_int32_t	tcpi_fackets;

  // Times. 
  u_int32_t	tcpi_last_data_sent;
  u_int32_t	tcpi_last_ack_sent;	//Not remembered, sorry.  
  u_int32_t	tcpi_last_data_recv;
  u_int32_t	tcpi_last_ack_recv;

  //Metrics.
  u_int32_t	tcpi_pmtu;
  u_int32_t	tcpi_rcv_ssthresh;
  u_int32_t	tcpi_rtt;
  u_int32_t	tcpi_rttvar;
  u_int32_t	tcpi_snd_ssthresh;
  u_int32_t	tcpi_snd_cwnd;
  u_int32_t	tcpi_advmss;
  u_int32_t	tcpi_reordering;
};
*/

#ifdef __cplusplus
}
#endif

#endif /** __QKC_NETINET_TCP_H */
