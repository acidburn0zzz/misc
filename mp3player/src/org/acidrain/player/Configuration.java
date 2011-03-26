package org.acidrain.player;

import java.io.File;
import java.io.Serializable;
import java.util.Hashtable;
import java.util.Vector;

/********
 * Cette classe ne sert qu'a stocker les parametres du logiciel
 * et les playlists pour pouvoir serialiser le tout simplement
 */
public class Configuration implements Serializable {
    private static final long serialVersionUID = 2L;

    public enum OSName {LINUX, WINDOWS, OSX}

    private Vector<File>[] listesFichiers;
    private String[] noms;
    private int[] nosChansonsJouees;
    private Hashtable<String, String> params;

    private transient static OSName osName;
    private transient static char separator;
    private transient static String home;

    static {
        String os = System.getProperty("os.name").toLowerCase();

        if (os.contains("linux")) {
            osName = OSName.LINUX;
            separator = '/';
        } else if (os.contains("win")) {
            osName = OSName.WINDOWS;
            separator = '\\';
        } else if (os.contains("osx")) {
            osName = OSName.OSX;
            separator = '/';
        }

        home = System.getProperty("user.home");
    }

    public Vector<File>[] getListesFichiers() {
        return listesFichiers;
    }

    public void setListesFichiers(Vector<File>[] listesFichiers) {
        this.listesFichiers = listesFichiers;
    }

    public String[] getNoms() {
        return noms;
    }

    public void setNoms(String[] noms) {
        this.noms = noms;
    }

    public int[] getNosChansonsJouees() {
        return nosChansonsJouees;
    }

    public void setNosChansonsJouees(int[] nosChansonsJouees) {
        this.nosChansonsJouees = nosChansonsJouees;
    }

    public Hashtable<String, String> getParams() {
        return params;
    }

    public void setParams(Hashtable<String, String> params) {
        this.params = params;
    }

    /*** OS RELATED ***/
    public static OSName getOsName() {
        return osName;
    }

    public static String getHomeDir() {
        return home;
    }

    public static char getSeparator() {
        return separator;
    }

    public static String getConfigurationPath() {
        if (osName == OSName.LINUX) {
            return home + separator + ".config" + separator + "acidmp3.cfg";
        } else if (osName == OSName.WINDOWS) {
            return home + separator + "acidmp3.cfg";
        } else if (osName == OSName.OSX) {
            return home + separator + ".config" + separator + "acidmp3.cfg";
        } else {
            return "config";
        }
    }
}
