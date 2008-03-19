using GLib;
using Gtk;

class TimeBarTest : Gtk.Window
{
  private TimeBar tb;
  construct {
    tb = new TimeBar();
    add(tb);
    show_all();
    destroy += Gtk.main_quit;
  }

  public static void main (string[] args)
  {
    Gtk.init (ref args);
    new TimeBarTest();    
  }
}
