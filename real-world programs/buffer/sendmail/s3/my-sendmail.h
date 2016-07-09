
/*


extern int mime_fromqp(u_char *, u_char ** ,int,int);

/* I have cut out the BITMAP field of header */
struct header
{
	char		*h_field;	/* the name of the field */
	char		*h_value;	/* the value of that field */
	struct header	*h_link;	/* the next header */
	u_short		h_flags;	/* status bits, see below */

};

typedef struct header	HDR;

/* modified address structure */
struct address
{
	char		*q_paddr;	/* the printname for the address */
	char		*q_user;	/* user name */
	char		*q_ruser;	/* real user name, or NULL if q_user */
	char		*q_host;	/* host name */
        /*struct mailer	*q_mailer;*/	/* mailer to use */
	u_long		q_flags;	/* status flags, see below */
	uid_t		q_uid;		/* user-id of receiver (if known) */
	gid_t		q_gid;		/* group-id of receiver (if known) */
	char		*q_home;	/* home dir (local mailer only) */
	char		*q_fullname;	/* full name if known */
	struct address	*q_next;	/* chain */
	struct address	*q_alias;	/* address this results from */
	char		*q_owner;	/* owner of q_alias */
	struct address	*q_tchain;	/* temporary use chain */
	char		*q_orcpt;	/* ORCPT parameter from RCPT TO: line */
	char		*q_status;	/* status code for DSNs */
	char		*q_rstatus;	/* remote status message for DSNs */
        /*time_t	q_statdate; */	/* date of status messages */
	char		*q_statmta;	/* MTA generating q_rstatus */
	short		q_specificity;	/* how "specific" this address is */
};

typedef struct address ADDRESS;


/* modified envelope structure */
struct envelope
{
	HDR		*e_header;	/* head of header list */
	long		e_msgpriority;	/* adjusted priority of this message */
	time_t		e_ctime;	/* time message appeared in the queue */
	char		*e_to;		/* the target person */
	ADDRESS		e_from;		/* the person it is from */
	char		*e_sender;	/* e_from.q_paddr w comments stripped */
	char		**e_fromdomain;	/* the domain part of the sender */
	ADDRESS		*e_sendqueue;	/* list of message recipients */
	ADDRESS		*e_errorqueue;	/* the queue for error responses */
	long		e_msgsize;	/* size of the message in bytes */
	long		e_flags;	/* flags, see below */
	int		e_nrcpts;	/* number of recipients */
	short		e_class;	/* msg class (priority, junk, etc.) */
	short		e_hopcount;	/* number of times processed */
	short		e_nsent;	/* number of sends since checkpoint */
	short		e_sendmode;	/* message send mode */
	short		e_errormode;	/* error return mode */
	short		e_timeoutclass;	/* message timeout class */
	struct envelope	*e_parent;	/* the message this one encloses */
	struct envelope *e_sibling;	/* the next envelope of interest */
	char		*e_bodytype;	/* type of message body */
	FILE		*e_dfp;		/* temporary file */
	char		*e_id;		/* code for this entry in queue */
	FILE		*e_xfp;		/* transcript file */
	FILE		*e_lockfp;	/* the lock file for this message */
	char		*e_message;	/* error message */
	char		*e_statmsg;	/* stat msg (changes per delivery) */
	char		*e_msgboundary;	/* MIME-style message part boundary */
	char		*e_origrcpt;	/* original recipient (one only) */
	char		*e_envid;	/* envelope id from MAIL FROM: line */
	char		*e_status;	/* DSN status for this message */
	time_t		e_dtime;	/* time of last delivery attempt */
	int		e_ntries;	/* number of delivery attempts */
	dev_t		e_dfdev;	/* df file's device, for crash recov */
	ino_t		e_dfino;	/* df file's ino, for crash recovery */
	char		*e_macro[256];	/* macro definitions */
};


typedef struct envelope	ENVELOPE;


# define bitset(bit, word)     (((word) & (bit)) != 0)
# define H_DEFAULT	0x0004	/* if another value is found, drop this */
# define MAXLINE 50            /* modified max line length */

extern char  *xalloc(int);
extern void mime7to8(HDR *, ENVELOPE *);

/* make a copy of a string */
#define newstr(s)	strcpy(xalloc(strlen(s) + 1), s)


/*

</source>

*/

