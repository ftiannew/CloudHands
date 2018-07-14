/*
 *
 *      Filename: do_smon_other.c
 *
 *        Author: shajf,csp001314@gmail.com
 *   Description: ---
 *        Create: 2018-05-17 17:41:03
 * Last Modified: 2018-07-17 17:01:06
 */


static void * do_smon_session_entry_create(ch_session_entry_t *sentry ch_unused){

	ch_pool_t *mp;

	ch_smon_session_entry_t *smon_entry = NULL;

	mp = ch_pool_create(512);

	if(mp == NULL){
	
		ch_log(CH_LOG_ERR,"Cannot create memory pool for smon session entry!");
		return NULL;
	}

	smon_entry = (ch_smon_session_entry_t*)ch_pcalloc(mp,sizeof(*smon_entry));

	ch_smon_session_entry_init(smon_entry,mp);


	return (void*)smon_entry;
}

#define SMON_ENTRY_CAN_STORE(entry) ((entry)->req_content_fpath!=NULL || (entry)->res_content_fpath!=NULL)


static void do_smon_session_entry_clean(ch_session_entry_t *sentry){

	ch_smon_session_entry_t *smon_entry = ch_my_session_entry_get(sentry,ch_smon_session_entry_t);
	ch_smon_parse_context_t *smon_parse_context = (ch_smon_parse_context_t*)sentry->pp_parser->parse_context;
	ch_session_monitor_t *monitor = &smon_parse_context->monitor;
	ch_session_monitor_item_t *item;

	if (SMON_ENTRY_CAN_STORE(smon_entry)) {

		item = ch_session_monitor_item_find(monitor,
			sentry->src_ip,sentry->dst_ip,sentry->src_port,sentry->dst_port);
		
		if(item == NULL){
			item = ch_session_monitor_item_find_ignore_state(monitor,
			sentry->src_ip,sentry->dst_ip,sentry->src_port,sentry->dst_port);
		}

		if(item)
			smon_entry->id = item->id;

		ch_session_store_put(sentry,(void*)smon_entry);
	}
	
	ch_smon_session_entry_fin(smon_entry);

	ch_pool_destroy(smon_entry->mp);

}
