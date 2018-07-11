#include "globus_gridftp_server.h"

static
globus_version_t local_version =
{
	0, /* major version number */
	1, /* minor version number */
	1531324167,
	0 /* branch ID */
};

typedef struct stub_handle_s
{
	int some_needed_data;
} stub_handle_t;

/*************************************************************************
 *  start
 *  -----
 *  This function is called when a new session is initialized, ie a user 
 *  connectes to the server.  This hook gives the dsi an oppertunity to
 *  set internal state that will be threaded through to all other
 *  function calls associated with this session.  And an oppertunity to
 *  reject the user.
 *
 *  finished_info.info.session.session_arg should be set to an DSI
 *  defined data structure.  This pointer will be passed as the void *
 *  user_arg parameter to all other interface functions.
 * 
 *  NOTE: at nice wrapper function should exist that hides the details 
 *        of the finished_info structure, but it currently does not.  
 *        The DSI developer should jsut follow this template for now
 ************************************************************************/
static void
stub_start(
    globus_gfs_operation_t      op,
    globus_gfs_session_info_t * session_info)
{

	stub_handle_t *       stub_handle;
	globus_gfs_finished_info_t          finished_info;
	GlobusGFSName(stub_start);

	stub_handle = (stub_handle_t *) globus_malloc(sizeof(stub_handle_t));

	stub_handle->some_needed_data = 0;

	memset(&finished_info, '\0', sizeof(globus_gfs_finished_info_t));
	finished_info.type = GLOBUS_GFS_OP_SESSION_START;
	finished_info.result = GLOBUS_SUCCESS;
	finished_info.info.session.session_arg = stub_handle;
	finished_info.info.session.username = session_info->username;
	finished_info.info.session.home_dir = "/";

	globus_gridftp_server_operation_finished(op,
	                                         GLOBUS_SUCCESS,
	                                         &finished_info);
}

/*************************************************************************
 *  destroy
 *  -------
 *  This is called when a session ends, ie client quits or disconnects.
 *  The dsi should clean up all memory they associated wit the session
 *  here. 
 ************************************************************************/
static void
stub_destroy(void * user_arg)
{
	stub_handle_t * stub_handle;

	stub_handle = (stub_handle_t *) user_arg;

	globus_free(stub_handle);
}

/*************************************************************************
 *  stat
 *  ----
 *  This interface function is called whenever the server needs 
 *  information about a given file or resource.  It is called then an
 *  LIST is sent by the client, when the server needs to verify that 
 *  a file exists and has the proper permissions, etc.
 ************************************************************************/
static void
stub_stat(
    globus_gfs_operation_t              op,
    globus_gfs_stat_info_t *            stat_info,
    void *                              user_arg)
{
	globus_gfs_stat_t                   stat_array[1];
	int                                 stat_count = 1;
	stub_handle_t *       stub_handle;
	GlobusGFSName(stub_stat);

	stub_handle = (stub_handle_t *) user_arg;

	stat_array[0].mode = 0;
	stat_array[0].nlink = 0;
	stat_array[0].uid = 0;
	stat_array[0].gid = 0;
	stat_array[0].size = 0;
	stat_array[0].mtime = 0;
	stat_array[0].atime = 0;
	stat_array[0].ctime = 0;
	stat_array[0].dev = 0;
	stat_array[0].ino = 0;

	globus_gridftp_server_finished_stat(op,
	                                    GLOBUS_SUCCESS, 
	                                    stat_array, 
	                                    stat_count);
}

/*************************************************************************
 *  command
 *  -------
 *  This interface function is called when the client sends a 'command'.
 *  commands are such things as mkdir, remdir, delete.  The complete
 *  enumeration is below.
 *
 *  To determine which command is being requested look at:
 *      cmd_info->command
 *
 *      GLOBUS_GFS_CMD_MKD = 1,
 *      GLOBUS_GFS_CMD_RMD,
 *      GLOBUS_GFS_CMD_DELE,
 *      GLOBUS_GFS_CMD_RNTO,
 *      GLOBUS_GFS_CMD_RNFR,
 *      GLOBUS_GFS_CMD_CKSM,
 *      GLOBUS_GFS_CMD_SITE_CHMOD,
 *      GLOBUS_GFS_CMD_SITE_DSI
 ************************************************************************/
static
void
stub_command(
    globus_gfs_operation_t              op,
    globus_gfs_command_info_t *         cmd_info,
    void *                              user_arg)
{
    stub_handle_t *       stub_handle;
    GlobusGFSName(stub_command);

    stub_handle = (stub_handle_t *) user_arg;

    globus_gridftp_server_finished_command(op, GLOBUS_SUCCESS, GLOBUS_NULL);
}

/*************************************************************************
 *  recv
 *  ----
 *  This interface function is called when the client requests that a
 *  file be transfered to the server.
 *
 *  To receive a file the following functions will be used in roughly
 *  the presented order.  They are doced in more detail with the
 *  gridftp server documentation.
 *
 *      globus_gridftp_server_begin_transfer();
 *      globus_gridftp_server_register_read();
 *      globus_gridftp_server_finished_transfer();
 *
 ************************************************************************/
static
void
stub_recv(
    globus_gfs_operation_t              op,
    globus_gfs_transfer_info_t *        transfer_info,
    void *                              user_arg)
{
    stub_handle_t * stub_handle;
    GlobusGFSName(stub_recv);

    stub_handle = (stub_handle_t *) user_arg;

    globus_gridftp_server_finished_transfer(op, GLOBUS_SUCCESS);
}

/*************************************************************************
 *  send
 *  ----
 *  This interface function is called when the client requests to receive
 *  a file from the server.
 *
 *  To send a file to the client the following functions will be used in roughly
 *  the presented order.  They are doced in more detail with the
 *  gridftp server documentation.
 *
 *      globus_gridftp_server_begin_transfer();
 *      globus_gridftp_server_register_write();
 *      globus_gridftp_server_finished_transfer();
 *
 ************************************************************************/
static
void
stub_send(
    globus_gfs_operation_t              op,
    globus_gfs_transfer_info_t *        transfer_info,
    void *                              user_arg)
{
    stub_handle_t * stub_handle;
    GlobusGFSName(stub_send);

    stub_handle = (stub_handle_t *) user_arg;

    globus_gridftp_server_finished_transfer(op, GLOBUS_SUCCESS);
}

static
int
stub_activate(void);

static
int
stub_deactivate(void);

/*
 *  no need to change this
 */
static globus_gfs_storage_iface_t       stub_dsi_iface = 
{
	GLOBUS_GFS_DSI_DESCRIPTOR_BLOCKING | GLOBUS_GFS_DSI_DESCRIPTOR_SENDER,
	stub_start,
	stub_destroy,
	NULL, /* list */
	stub_send,
	stub_recv,
	NULL, /* trev */
	NULL, /* active */
	NULL, /* passive */
	NULL, /* data destroy */
	stub_command, 
	stub_stat,
	NULL,
	NULL
};

/*
 *  no need to change this
 */
GlobusExtensionDefineModule(globus_gridftp_server_stub) =
{
	"globus_gridftp_server_stub",
	stub_activate,
	stub_deactivate,
	NULL,
	NULL,
	&local_version
};

/*
 *  no need to change this
 */
static int
stub_activate(void)
{
	globus_extension_registry_add(
	    GLOBUS_GFS_DSI_REGISTRY,
	    "stub",
	    GlobusExtensionMyModule(globus_gridftp_server_stub),
	    &stub_dsi_iface);
    
	return 0;
}

/*
 *  no need to change this
 */
static int
stub_deactivate(void)
{
	globus_extension_registry_remove(GLOBUS_GFS_DSI_REGISTRY, "stub");
	return 0;
}

int
main()
{
	return 0;
}
