package org.acidrain.player;

import java.io.File;
import java.io.Serializable;
import java.util.Hashtable;
import java.util.Vector;

public class Configuration implements Serializable {
     private static final long serialVersionUID = 2L;
    /********
     * Cette classe ne sert qu'a stocker les parametres du logiciel
     * et les playlists pour pouvoir serialiser le tout simplement
     */
    private Vector<File>[] listesFichiers;
    private String[] noms;
    private int[] nosChansonsJouees;
    private Hashtable<String, String> params;
    
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
}
