{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 9,
			"minor" : 0,
			"revision" : 0,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 240.0, 509.0, 640.0, 480.0 ],
		"gridsize" : [ 15.0, 15.0 ],
		"boxes" : [ 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-5",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "midi_out.maxpat",
					"numinlets" : 2,
					"numoutlets" : 0,
					"offset" : [ 0.0, 0.0 ],
					"patching_rect" : [ 542.5390625, 215.46875, 99.0, 22.0 ],
					"varname" : "midiout",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-4",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "midi_in.maxpat",
					"numinlets" : 2,
					"numoutlets" : 1,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "" ],
					"patching_rect" : [ 433.0, 68.0, 99.0, 22.0 ],
					"varname" : "midiin",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 284.0, 328.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"blanksym" : "",
					"id" : "obj-9",
					"items" : [ "None", ",", "Network Session 1", ",", "IAC Driver Bus 1", ",", "Sensel Morph", ",", "iPad", ",", "from Max 1", ",", "from Max 2", ",", "<none>" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 289.0, 411.0, 100.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 284.0, 371.0, 96.0, 22.0 ],
					"text" : "aumhaa.midi4lin"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 268.0, 86.0, 24.0, 24.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"items" : [ "None", ",", "IAC Bus Bus 1", ",", "Network Book Session", ",", "from Max 1", ",", "from Max 2", ",", "success2" ],
					"maxclass" : "umenu",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "int", "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 312.0, 227.0, 100.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 92.0, 223.0, 105.0, 22.0 ],
					"saved_object_attributes" : 					{
						"filename" : "midi4l_js_test.js",
						"parameter_enable" : 0
					}
,
					"text" : "js midi4l_js_test.js"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 283.0, 139.0, 96.0, 22.0 ],
					"text" : "aumhaa.midi4lin",
					"varname" : "in"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"source" : [ "obj-1", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-8", 1 ]
				}

			}
 ],
		"originid" : "pat-4",
		"parameters" : 		{
			"obj-4::obj-2" : [ "portlist", "portlist", 0 ],
			"obj-5::obj-2" : [ "portlist[1]", "portlist", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "_base.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa.midi4lin.mxo",
				"type" : "iLaX"
			}
, 			{
				"name" : "aumhaa_LiveAPI_util.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/base",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/base",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_bindable.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/base",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/base",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_button_class.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_chooser.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_control_class.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_control_registry.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_control_surface.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_dict_module.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_event_emitter.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_filenames.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_floating_window.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_global_proxy.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_grid_class.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_gui_button_class.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_gui_control.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_gui_element.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_midi_event_router.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_midi_io_module.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_mira_grid.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_mod.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_modes.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_multi_notifier.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_node_script_proxy.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_notification_display.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_notifier_class.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_notifier_consts.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_parameters.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_preset_component.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_promise.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/base",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/base",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_scales.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_settings_obj.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/notifiers",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/notifiers",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_task_server.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/components",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/components",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "aumhaa_util.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/aumhaa_js/base",
				"patcherrelativepath" : "../mod/javascript/aumhaa_js/base",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "midi4l_js_test.js",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/tests",
				"patcherrelativepath" : "../mod/javascript/tests",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "midi_in.maxpat",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/utility",
				"patcherrelativepath" : "../mod/javascript/utility",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "midi_out.maxpat",
				"bootpath" : "~/Documents/Max 9/Packages/mod/javascript/utility",
				"patcherrelativepath" : "../mod/javascript/utility",
				"type" : "JSON",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
