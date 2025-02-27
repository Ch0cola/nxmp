#include <algorithm>
#include <iostream>

#include "gui.h"
#include "localfiles.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

MenuItem item;

namespace GUI {
	enum SDL_KEYS {
		SDL_KEY_A, SDL_KEY_B, SDL_KEY_X, SDL_KEY_Y,
		SDL_KEY_LSTICK, SDL_KEY_RSTICK,
		SDL_KEY_L, SDL_KEY_R,
		SDL_KEY_ZL, SDL_KEY_ZR,
		SDL_KEY_PLUS, SDL_KEY_MINUS,
		SDL_KEY_DLEFT, SDL_KEY_DUP, SDL_KEY_DRIGHT, SDL_KEY_DDOWN,
		SDL_KEY_LSTICK_LEFT, SDL_KEY_LSTICK_UP, SDL_KEY_LSTICK_RIGHT, SDL_KEY_LSTICK_DOWN,
		SDL_KEY_RSTICK_LEFT, SDL_KEY_RSTICK_UP, SDL_KEY_RSTICK_RIGHT, SDL_KEY_RSTICK_DOWN,
		SDL_KEY_SL_LEFT, SDL_KEY_SR_LEFT, SDL_KEY_SL_RIGHT, SDL_KEY_SR_RIGHT
	};
	
	static void on_mpv_events(void *ctx)
	{
		SDL_Event event = {.type = wakeup_on_mpv_events};
		SDL_PushEvent(&event);
	}
	
	void initMpv(){
		libmpv = new libMpv("mpv");
		wakeup_on_mpv_render_update = SDL_RegisterEvents(1);
		wakeup_on_mpv_events        = SDL_RegisterEvents(1);
		mpv_set_wakeup_callback(libmpv->getHandle(), on_mpv_events, NULL);
		mpv_render_context_set_update_callback(libmpv->getContext(), [](void *) -> void { SDL_Event event = {.type = wakeup_on_mpv_render_update}; SDL_PushEvent(&event); }, NULL);
	
	}
	
	void toggleStats(){
		const char *cmd[] = {"script-binding","stats/display-stats-toggle" ,NULL};
		mpv_command_async(libmpv->getHandle(), 0, cmd);
	}
	
	void toggleMasterLock(){
		item.masterlock = !item.masterlock;
		if(item.masterlock){
			const char *cmd[] = {"show-text", "Masterlock Enabled","2000", NULL};
			mpv_command_async(libmpv->getHandle(), 0, cmd);
		}else{
			const char *cmd[] = {"show-text", "Masterlock Disabled","2000", NULL};
			mpv_command_async(libmpv->getHandle(), 0, cmd);
		}
		
	}
	
	
	void HandleEvents(){
		SDL_Event event;
			while (SDL_PollEvent(&event)) {
				ImGui_ImplSDL2_ProcessEvent(&event);
#ifndef __SWITCH__
				if (event.type == SDL_KEYDOWN) {
					Uint8 keycode = event.key.keysym.sym;
					//printf("Key code :%d\n",keycode);
					if(keycode == SDLK_ESCAPE){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_PLUS;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == 79){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_DRIGHT;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == 80){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_DLEFT;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == 82){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_DUP;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == 81){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_DDOWN;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == 32){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_A;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == SDLK_y){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_Y;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == SDLK_b){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_B;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == SDLK_a){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_A;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == SDLK_x){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_X;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == SDLK_r){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_R;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == SDLK_l){
						SDL_Event sdlevent;
						sdlevent.type = SDL_JOYBUTTONDOWN;
						sdlevent.jbutton.button = SDL_KEY_L;
						SDL_PushEvent(&sdlevent);
					}
					if(keycode == SDLK_t){
						//mpv_set_option_string(libmpv->getHandle(), "gpu-nxmp-deint", "0");
						//mpv_set_option_string(libmpv->getHandle(), "glsl-shader", "C:\\Users\\Ceco\\C-Dev\\imgui-test\\mpv\\shaders\\test.hook");
						mpv_command_string(libmpv->getHandle(),"no-osd change-list glsl-shaders set C:\\Users\\Ceco\\C-Dev\\imgui-test\\mpv\\shaders\\scale.glsl");
					}
					if(keycode == SDLK_e){
						mpv_command_string(libmpv->getHandle(),"no-osd change-list glsl-shaders clr \"\"");
						//mpv_set_option_string(libmpv->getHandle(), "gpu-nxmp-deint", "1");
					}
					
				}
#endif
				if (event.type == SDL_JOYBUTTONDOWN) {
					
					Uint8 button = event.jbutton.button;
					if (button == SDL_KEY_PLUS && !item.masterlock)
						renderloopdone = true;
					
					if (button == SDL_KEY_DUP){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock && item.rightmenustate == PLAYER_RIGHT_MENU_PLAYER && item.popupstate == POPUP_STATE_NONE){
						
						}
					}
					
					if (button == SDL_KEY_DDOWN){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock && item.rightmenustate == PLAYER_RIGHT_MENU_PLAYER && item.popupstate == POPUP_STATE_NONE){
							if(item.playercontrolstate == PLAYER_CONTROL_STATE_NONE){
								item.playercontrolstate = PLAYER_CONTROL_STATE_CONTROLS;
							}else{
								item.playercontrolstate = PLAYER_CONTROL_STATE_NONE;
							}
						}
					}
					
					if (button == SDL_KEY_DRIGHT){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock && item.playercontrolstate != PLAYER_CONTROL_STATE_CONTROLS && item.popupstate == POPUP_STATE_NONE){
							if(item.rightmenustate == PLAYER_RIGHT_MENU_PLAYER){
								if(item.rightmenustate != PLAYER_RIGHT_MENU_CUSTOMARATIO && item.rightmenustate != PLAYER_RIGHT_MENU_IMAGE && item.rightmenustate != PLAYER_RIGHT_MENU_AUDIO && item.rightmenustate != PLAYER_RIGHT_MENU_SUB && item.rightmenustate != PLAYER_AUDIOEQ && item.rightmenustate != PLAYER_SUPERAUDIOEQ){
									item.rightmenustate = PLAYER_RIGHT_MENU_HOME;
								}
							}else if(item.rightmenustate != PLAYER_RIGHT_MENU_PLAYER){
								if(item.rightmenustate != PLAYER_RIGHT_MENU_CUSTOMARATIO && item.rightmenustate != PLAYER_RIGHT_MENU_IMAGE && item.rightmenustate != PLAYER_RIGHT_MENU_AUDIO && item.rightmenustate != PLAYER_RIGHT_MENU_SUB && item.rightmenustate != PLAYER_AUDIOEQ && item.rightmenustate != PLAYER_SUPERAUDIOEQ){
									item.rightmenustate = PLAYER_RIGHT_MENU_PLAYER;
									item.rightmenu_startpos = 1280.0f;
								}
							}
						}
					}
					
					if (button == SDL_KEY_DLEFT){
						
					}
					
					if (button == SDL_KEY_RSTICK){
						if(item.state == MENU_STATE_PLAYER){
							toggleMasterLock();
						}
					}
					if (button == SDL_KEY_MINUS){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock){
							toggleStats();
						}
					}
					
					if (button == SDL_KEY_ZR){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock){
							libmpv->seek(libmpv->getPosition() + configini->getLongSeek(false),item.playershowcontrols);
						}
						
					}
					if (button == SDL_KEY_ZL){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock){
							libmpv->seek(libmpv->getPosition() - configini->getLongSeek(false),item.playershowcontrols);
						
						}
						
					}
					if (button == SDL_KEY_R){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock){
							libmpv->seek(libmpv->getPosition() + configini->getShortSeek(false),item.playershowcontrols);
						
						}
						
					}
					if (button == SDL_KEY_L){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock){
							libmpv->seek(libmpv->getPosition() - configini->getShortSeek(false),item.playershowcontrols);
						
						}
						
					}
					if (button == SDL_KEY_X){
						if(item.state == MENU_STATE_PLAYER && !item.masterlock){
							if(item.playershowcontrols){
								item.playershowcontrols=false;
								mpv_set_option_string(libmpv->getHandle(), "osd-level", "0");
								
							}else{
								item.playershowcontrols=true;
								mpv_set_option_string(libmpv->getHandle(), "osd-level", "3");
							}
						}
					}
					if (button == SDL_KEY_A){
						if(item.state == MENU_STATE_PLAYER){
							if(item.rightmenustate == PLAYER_RIGHT_MENU_PLAYER && item.playercontrolstate == PLAYER_CONTROL_STATE_NONE && item.popupstate == POPUP_STATE_NONE){
								if(libmpv->Paused()){
									libmpv->Resume();
								}else{
									libmpv->Pause();
								}
							}
						}
					}
					if (button == SDL_KEY_Y && !item.masterlock){
						if(item.state != MENU_STATE_HOME && item.state != MENU_STATE_PLAYER && item.popupstate == POPUP_STATE_NONE){
							if(localdir != nullptr){
								delete localdir;
								localdir = nullptr;
							}
#ifdef __SWITCH__
							if(usbmounter != nullptr){
								delete usbmounter;
								usbmounter = nullptr;
							}
#endif
							if(ftpdir != nullptr){
								delete ftpdir;
								ftpdir = nullptr;
							}
							if(httpdir != nullptr){
								delete httpdir;
								httpdir = nullptr;
							}
							if(enigma2 != nullptr){
								delete enigma2;
								enigma2 = nullptr;
							}
							item.networkselect = true;
							item.first_item = true;
							item.state = MENU_STATE_HOME;
							item.usbpath = "";
							
						}
										
					}
					if (button == SDL_KEY_B){
						if(item.state == MENU_STATE_ENIGMABROWSER && libmpv->Stopped()){
							item.first_item = true;
							enigma2->backToTop();
						}
						
						if(item.state == MENU_STATE_FTPBROWSER && libmpv->Stopped()){
							item.first_item = true;
							ftpdir->backDir();
							ftpdir->DirList(ftpdir->getCurrPath(),Utility::getMediaExtensions());
						}
						
						if(item.state == MENU_STATE_HTTPBROWSER && libmpv->Stopped()){
							item.first_item = true;
							httpdir->backDir();
							httpdir->DirList(httpdir->getCurrPath(),Utility::getMediaExtensions());
							
						}
						
						if(item.state == MENU_STATE_USB && libmpv->Stopped()){
							if(item.usbbasepath != ""){
								item.first_item = true;
								if(item.usbpath != item.usbbasepath){
									item.usbpath = item.usbpath.substr(0, item.usbpath.find_last_of("\\/"));
									if(item.usbpath == "")item.usbpath=item.usbbasepath;
								}
								
								item.usbfileentries = FS::getDirList(item.usbpath.c_str(),true,Utility::getMediaExtensions());
							}
						}
						
						if(item.state == MENU_STATE_FILEBROWSER && libmpv->Stopped()){
							item.first_item = true;
							localdir->backPath();
							localdir->DirList(localdir->getCurrentPath(),true,Utility::getMediaExtensions());
							
						}
						
						if(item.state == MENU_STATE_PLAYER || item.state == MENU_STATE_PLAYERCACHING){
							if(item.rightmenustate == PLAYER_RIGHT_MENU_PLAYER && item.playercontrolstate == PLAYER_CONTROL_STATE_NONE){
								if(!libmpv->Stopped()  && !item.masterlock){
									item.state = item.laststate;
									
									libmpv->Stop();
								}
							}else if(item.rightmenustate == PLAYER_RIGHT_MENU_TRACKS){
								item.rightmenustate = PLAYER_RIGHT_MENU_HOME;
							}
							else if(item.rightmenustate == PLAYER_RIGHT_MENU_TRACKS_VIDEO||item.rightmenustate == PLAYER_RIGHT_MENU_TRACKS_AUDIO||item.rightmenustate == PLAYER_RIGHT_MENU_TRACKS_SUB){
								item.rightmenustate = PLAYER_RIGHT_MENU_TRACKS;
							}
							else if(item.rightmenustate == PLAYER_RIGHT_MENU_CHAPTERS){
								item.rightmenustate = PLAYER_RIGHT_MENU_HOME;
							}
							else if(item.rightmenustate == PLAYER_RIGHT_MENU_ARATIO){
								item.rightmenustate = PLAYER_RIGHT_MENU_HOME;
							}
							else if(item.rightmenustate == PLAYER_RIGHT_MENU_CUSTOMARATIO){
								item.rightmenustate = PLAYER_RIGHT_MENU_ARATIO;
							}
							else if(item.rightmenustate == PLAYER_RIGHT_MENU_IMAGE){
								item.rightmenustate = PLAYER_RIGHT_MENU_HOME;
							}
							else if(item.rightmenustate == PLAYER_RIGHT_MENU_AUDIO){
								item.rightmenustate = PLAYER_RIGHT_MENU_HOME;
							}
							else if(item.rightmenustate == PLAYER_RIGHT_MENU_SUB){
								item.rightmenustate = PLAYER_RIGHT_MENU_HOME;
							}
							else if(item.rightmenustate == PLAYER_AUDIOEQ){
								item.rightmenustate = PLAYER_RIGHT_MENU_AUDIO;
							}
							else if(item.rightmenustate == PLAYER_SUPERAUDIOEQ){
								item.rightmenustate = PLAYER_RIGHT_MENU_AUDIO;
							}
							
							
						}
					}
				}
				if (event.type == wakeup_on_mpv_render_update)
				{
					mpv_render_context_update(libmpv->getContext());
				}
				if (event.type == wakeup_on_mpv_events)
				{
					while (1) {
					mpv_event *mp_event = (mpv_event*) mpv_wait_event(libmpv->getHandle(), 0);
					if (mp_event->event_id == MPV_EVENT_NONE)
						break;
					if (mp_event->event_id == MPV_EVENT_LOG_MESSAGE) {
						mpv_event_log_message *msg = (mpv_event_log_message *) mp_event->data;
						continue;
					}
					if (mp_event->event_id == MPV_EVENT_FILE_LOADED) {
						libmpv->getfileInfo();
						item.state = MENU_STATE_PLAYER;
						if(libmpv->getFileInfo()->videos.size() == 0 || (libmpv->getFileInfo()->videos.size() == 1 && libmpv->getFileInfo()->videos[0].albumart) ){
							item.playerstate = PLAYER_STATE_AUDIO;
							mpv_observe_property(libmpv->getHandle(), 0, "playback-time", MPV_FORMAT_DOUBLE);
							mpv_observe_property(libmpv->getHandle(), 0, "duration", MPV_FORMAT_DOUBLE);
							mpv_observe_property(libmpv->getHandle(), 0, "metadata", MPV_FORMAT_NODE);
							item.playercontrolstate = PLAYER_CONTROL_STATE_CONTROLS;
							
						}else{
							mpv_observe_property(libmpv->getHandle(), 0, "playback-time", MPV_FORMAT_DOUBLE);
							mpv_observe_property(libmpv->getHandle(), 0, "duration", MPV_FORMAT_DOUBLE);
							mpv_observe_property(libmpv->getHandle(), 0, "metadata", MPV_FORMAT_NODE);
							item.playerstate = PLAYER_STATE_VIDEO;
						}
					}
					
					if (mp_event->event_id == MPV_EVENT_START_FILE) {
#ifdef __SWITCH__
						appletSetMediaPlaybackState(true);
#endif					
						printf("START FILE\n");
						item.laststate = item.state;
						item.state = MENU_STATE_PLAYERCACHING;
					}
					//printf("Event id: %d\n",mp_event->event_id);
					if (mp_event->event_id == MPV_EVENT_END_FILE) {
#ifdef __SWITCH__
						appletSetMediaPlaybackState(false);
#endif					
						struct mpv_event_end_file *eof = (struct mpv_event_end_file *)mp_event->data;
						libmpv->setLoop(false);
						if(item.playerstate == PLAYER_STATE_VIDEO  && libmpv->getFileInfo()->playbackInfo.islive == false && sqlitedb != nullptr){
							if(libmpv->getFileInfoPerc() >= configini->getResumeStartPerc(false) && 100-libmpv->getFileInfoPerc() >= configini->getResumeStopPerc(false)){
								sqlitedb->writeResume(libmpv->getFileInfo()->path,libmpv->getFileInfo()->playbackInfo.position);
							}else{
								if(eof->reason != MPV_END_FILE_REASON_EOF){
									sqlitedb->deleteResume(libmpv->getFileInfo()->path);
								}
							}
							if(eof->reason == MPV_END_FILE_REASON_EOF){
								printf("FILE EOF\n");
								sqlitedb->deleteResume(libmpv->getFileInfo()->path);
							}
						}
						printf("END PLAY FILE\n");
						item.state = item.laststate;
						item.rightmenustate = PLAYER_RIGHT_MENU_PLAYER;
						item.playercontrolstate = PLAYER_CONTROL_STATE_NONE;
						item.masterlock = false;
						printf("MENU STATE: %d\n",item.state );
					}
					if(mp_event->event_id == MPV_EVENT_PROPERTY_CHANGE){
						mpv_event_property *prop = (mpv_event_property*)mp_event->data;
						if(std::string(prop->name) == "metadata" && !libmpv->Stopped()) 
						{
							mpv_node node = *(mpv_node *)prop->data;
							printf("Node format %d\n",node.format);
							if (node.format == MPV_FORMAT_NODE_MAP) {
								for (int n = 0; n < node.u.list->num; n++) {
									std::string key = node.u.list->keys[n];
									if(key == "title"){
										libmpv->getFileInfo()->playbackInfo.title = node.u.list->values[n].u.string;
									}
									if(key == "artist"){
										libmpv->getFileInfo()->playbackInfo.artist = node.u.list->values[n].u.string;
									}
									printf("META KEY %s\n",key.c_str());
								}
							}
						}
						if(std::string(prop->name) == "playback-time") 
						{
							if(prop->format == MPV_FORMAT_DOUBLE)
							{
								double timepos = *(double *)prop->data;
								libmpv->getFileInfo()->playbackInfo.position = (int)timepos;
							}
						}
						if(std::string(prop->name) == "duration") 
						{
							if(prop->format == MPV_FORMAT_INT64)
							{
								int64_t timepos = *(int64_t *)prop->data;
								libmpv->getFileInfo()->playbackInfo.duration = timepos;
							}
						}
					}
					if(mp_event->event_id!=22){
						printf("event: %s %d\n", mpv_event_name(mp_event->event_id),mp_event->event_id);
					}
					}
				}
			}
	}
	
	void HandleLayers(){
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
			switch (item.state) {
				case MENU_STATE_HOME:
					Windows::MainMenuWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_FILEBROWSER:
					Windows::FileBrowserWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_USB:
					Windows::USBBrowserWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_NETWORKBROWSER:
					Windows::NetworkWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_FTPBROWSER:
					Windows::FtpWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_HTTPBROWSER:
					Windows::HttpWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_ENIGMABROWSER:
					Windows::EnigmaWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_SETTINGS:
					Windows::SettingsMenuWindow(&item.focus, &item.first_item);
					if(item.popupstate == POPUP_STATE_SAVE_SETTINGS){
						Popups::SaveSettingsPopup();
					}
					break;
				case MENU_STATE_INFO:
					Windows::InfoMenuWindow(&item.focus, &item.first_item);
					break;
				case MENU_STATE_PLAYERCACHING:
					playerWindows::CacheWindow();
					break;
				case MENU_STATE_PLAYER:
					if(item.popupstate == POPUP_STATE_RESUME){
						Popups::ResumePopup();
					}
				
					if(item.playerstate == PLAYER_STATE_AUDIO){
						
					}
					break;
				
			}
			
			switch (item.playercontrolstate) {
				case PLAYER_CONTROL_STATE_NONE:
					break;
				case PLAYER_CONTROL_STATE_CONTROLS:
					playerWindows::playerControls();
					break;
			}
			
			switch (item.rightmenustate) {
				case PLAYER_RIGHT_MENU_PLAYER:
					break;
				case PLAYER_RIGHT_MENU_HOME:
					playerWindows::RightHomeWindow(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_TRACKS:
					playerWindows::RightTrackWindow(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_TRACKS_VIDEO:
					playerWindows::RightTrackVideoWindow(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_TRACKS_AUDIO:
					playerWindows::RightTrackAudioWindow(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_TRACKS_SUB:
					playerWindows::RightTrackSubWindow(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_CHAPTERS:
					playerWindows::RightChapterWindow(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_ARATIO:
					playerWindows::RightHomeARatio(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_CUSTOMARATIO:
					playerWindows::RightHomeCustomARatio(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_IMAGE:
					playerWindows::RightHomeImage(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_AUDIO:
					playerWindows::RightHomeAudio(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_RIGHT_MENU_SUB:
					playerWindows::RightHomeSub(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;	
				case PLAYER_AUDIOEQ:
					playerWindows::AudioEqualizer(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;
				case PLAYER_SUPERAUDIOEQ:
					playerWindows::AudioSuperEqualizer(&item.rightmenu_focus,&item.rightmenu_first_item);
					break;	
					
			}
		
	}
	
	void HandleRender(){
		ImGui::Render();
		ImGuiIO &io = ImGui::GetIO();
		glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
		glClearColor(0.00f, 0.00f, 0.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);

		{
			int w, h;
			SDL_GetWindowSize(window, &w, &h);
			fbo = {
				.fbo = 0,
				.w = w,
				.h = h,
			};
			params[0] = {MPV_RENDER_PARAM_OPENGL_FBO, &fbo};
			params[1] = {MPV_RENDER_PARAM_FLIP_Y, &__fbo_one};
			params[2] = {(mpv_render_param_type)0};
			mpv_render_context_render(libmpv->getContext(), params);
		}
    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);

	}

	int RenderLoop(void) {
	
		while (!renderloopdone)
		{
			HandleEvents();
			HandleLayers();
			HandleRender();
		}
		
		return 0;	
	}

}	
					