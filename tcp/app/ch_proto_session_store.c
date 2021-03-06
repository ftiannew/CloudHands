/*
 *
 *      Filename: ch_proto_session_store.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-07-28 14:47:12
 * Last Modified: 2018-08-16 17:17:29
 */

#include "ch_proto_session_store.h"
#include "ch_log.h"
#include "ch_packet_record.h"

ch_proto_session_store_t *ch_proto_session_store_create(ch_pool_t *mp,uint32_t task_id,const char *shm_fname,
	uint64_t shm_fsize,uint64_t fentry_size){

	int i = 0;
	ch_proto_session_store_t *pstore = (ch_proto_session_store_t*)ch_palloc(mp,sizeof(*pstore));

	pstore->task_id = task_id;

	pstore->mp  = mp;
	pstore->shm_fmt = ch_shm_format_pkt_with_mmap_create(mp,shm_fname,shm_fsize,fentry_size,0,1);

	if(pstore->shm_fmt == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot Create shm store failed for proto session store!");
		return NULL;
	}
	
    if(ch_buffer_init(&pstore->g_buffer)){
	
		ch_log(CH_LOG_ERR,"Cannot create buffer for protocol session store!");
		return NULL;
	}

    msgpack_sbuffer_init(&pstore->pk_buf);
    msgpack_packer_init(&pstore->pk,&pstore->pk_buf,msgpack_sbuffer_write);

	ch_proto_body_store_t *bstore;

	for(i= 0;i<PROTOCOL_MAX;i++){
	
		bstore = &pstore->body_stores[i];
		bstore->req_fpath = NULL;
		bstore->res_fpath = NULL;
	}

	return pstore;

}

void ch_proto_session_store_destroy(ch_proto_session_store_t *pstore){

    msgpack_sbuffer_destroy(&pstore->pk_buf);

}

int ch_proto_session_store_write(ch_proto_session_store_t *pstore,ch_tcp_session_t *tsession,ch_proto_session_entry_t *session){

	void *data;
	size_t dlen;

	ch_tcp_app_t *app = tsession->app;

	msgpack_packer *pk = &pstore->pk;

	/*packer the common data into msgpack*/
	ch_msgpack_map_start(pk,NULL,2);
	ch_msgpack_map_start(pk,"common",14);

	ch_msgpack_write_uint32(pk,"protoID",app->protocol_id);
	ch_msgpack_write_uint64(pk,"sessionID",tsession->session_id);
	ch_msgpack_write_uint64(pk,"reqTime",ch_tcp_session_reqtime_get(tsession));
	ch_msgpack_write_uint64(pk,"resTime",ch_tcp_session_restime_get(tsession));
	ch_msgpack_write_uint32(pk,"srcIP",ch_tcp_session_srcip_get(tsession));
	ch_msgpack_write_uint32(pk,"dstIP",ch_tcp_session_dstip_get(tsession));
	ch_msgpack_write_uint16(pk,"srcPort",ch_tcp_session_srcport_get(tsession));
	ch_msgpack_write_uint16(pk,"dstPort",ch_tcp_session_dstport_get(tsession));
	ch_msgpack_write_uint64(pk,"reqPackets",session->reqPackets);
	ch_msgpack_write_uint64(pk,"reqBytes",session->reqBytes);
	ch_msgpack_write_uint64(pk,"reqPBytes",session->reqBytes);
	
	ch_msgpack_write_uint64(pk,"resPackets",session->resPackets);
	ch_msgpack_write_uint64(pk,"resBytes",session->resBytes);
	ch_msgpack_write_uint64(pk,"resPBytes",session->resBytes);

	/*packer the session data into msgpack*/
	app->proto_session_format(pk,(void*)session);

    data = pstore->pk_buf.data;
    dlen = pstore->pk_buf.size;


	ch_packet_record_t pkt_rcd;
	pkt_rcd.type = app->pkt_rcd_type;
	pkt_rcd.meta_data_size = 0;
	pkt_rcd.time = ch_tcp_session_reqtime_get(tsession);

	ch_packet_record_put(pstore->shm_fmt,&pkt_rcd,data,dlen);

    msgpack_sbuffer_clear(&pstore->pk_buf);

	/*ok*/
	return 0;

}

