/*

  Copyright (c) 2003,2004 uim Project http://uim.freedesktop.org/

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  3. Neither the name of authors nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.

*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdlib.h>
#include <locale.h>
#include <panel-applet.h>
#include <gnome.h>
#include <libgnome/libgnome.h>
#include <uim/uim.h>

PanelApplet *uimapplet;

static void
exec_switcher(BonoboUIComponent *uic, gpointer data, const gchar *verbname);
static void
exec_pref(BonoboUIComponent *uic, gpointer data, const gchar *verbname);
static void
display_about_dialog(BonoboUIComponent *uic, gpointer data, const gchar *verbname);

GtkWidget *uim_helper_applet_new(void);


static const BonoboUIVerb uim_menu_verbs [] = {
  BONOBO_UI_VERB ("UimExecSwitcher",   exec_switcher),
  BONOBO_UI_VERB ("UimExecPref",   exec_pref),
  BONOBO_UI_VERB ("UimAbout",      display_about_dialog),
  BONOBO_UI_VERB_END
};


static const char uim_menu_xml [] =
        "<popup name=\"button3\">\n"
        "   <menuitem name=\"Switcher Item\" verb=\"UimExecSwitcher\" _label=\"Execute uim's input method switcher\"\n"
        "             pixtype=\"filename\" pixname=\""UIM_PIXMAPSDIR"/switcher-icon.png\"/>\n"
        "   <menuitem name=\"Pref Item\" verb=\"UimExecPref\" _label=\"Execute uim's preference tool\"\n"
        "             pixtype=\"stock\" pixname=\"preferences\"/>\n"
        "   <menuitem name=\"About Item\" verb=\"UimAbout\" _label=\"About ...\"\n"
        "             pixtype=\"stock\" pixname=\"gnome-stock-about\"/>\n"
        "</popup>\n";



static void
exec_switcher(BonoboUIComponent *uic, gpointer data, const gchar *verbname)
{
  system("uim-im-switcher &");
}

static void
exec_pref(BonoboUIComponent *uic, gpointer data, const gchar *verbname)
{
  system("uim-pref-gtk &");
}


/* Opens gnome help application
 */
#if 0
static void 
display_help_dialog(BonoboUIComponent *uic, gpointer data, const gchar *verbname)
{
  GError *error = NULL;
  
  gnome_help_display("Uim", NULL, &error);

  if (error)
    {
      GtkWidget *dialog;
      dialog = gtk_message_dialog_new(NULL, 
				      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
				      GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
				      error->message);
      gtk_dialog_run (GTK_DIALOG (dialog));
      gtk_widget_destroy (dialog);
      g_error_free (error);
      error = NULL;
    }
}
#endif

/* Just the about window... If it's already open, just focus it
 */
static void
display_about_dialog(BonoboUIComponent *uic, gpointer data, const gchar *verbname)
{
  /*	GdkPixbuf *icon;*/
  GtkWidget *dialog;
  const char *authors[] = {"uim Project", NULL};
  
  /* Feel free to put your names here translators */
  char *translators = _("TRANSLATORS");
  
  /* icon = gdk_pixbuf_new_from_xpm_data(ICON_APPLET);*/
  
  dialog = GTK_WIDGET(gnome_about_new (_("Uim Applet for GNOME"), VERSION, "Copyright 2003-2005 uim Project.",
				       _("Applet for indicate uim's status"),
				       (const char **) authors, NULL, 
				       strcmp("TRANSLATORS", translators) ? translators : NULL, 
				       NULL));
  
  gtk_widget_show(GTK_WIDGET(dialog));
}


static gboolean
uim_applet_new(PanelApplet *applet,
		 const gchar *iid,
		 gpointer     data)
{
  GtkWidget *toolbar;
  uimapplet = applet;
 
  if (strcmp (iid, "OAFIID:GNOME_UimApplet") != 0)
    return FALSE;

  uim_init();

  toolbar = (GtkWidget*)uim_helper_applet_new();

  gtk_container_add (GTK_CONTAINER (applet), toolbar);  
  gtk_widget_show_all (GTK_WIDGET (applet));

  panel_applet_setup_menu(applet,
			  uim_menu_xml,
			  uim_menu_verbs,
			  toolbar);

  return TRUE;
}



PANEL_APPLET_BONOBO_FACTORY ("OAFIID:GNOME_UimApplet_Factory",
                             PANEL_TYPE_APPLET,
                             "uim Applet for GNOME",
                             "0",
                             (PanelAppletFactoryCallback)uim_applet_new,
                             NULL)
