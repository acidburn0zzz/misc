package org.acidrain.player;

import java.io.File;
import java.io.Serializable;

import org.acidrain.id3.ID3v1;
import org.acidrain.id3.ID3v2;


@SuppressWarnings("serial")
public class Chanson implements Serializable {
    private File file;
    private ID3v1 id3v1;
    private ID3v2 id3v2;
    
    public Chanson(File f) {
        file = f;

        id3v1 = new ID3v1(file);
        id3v2 = new ID3v2(file);
    }
    
    public Chanson(String s) {
        file = new File(s);

        id3v1 = new ID3v1(file);
        id3v2 = new ID3v2(file);
    }
    
    public File getFile() {
        return file;
    }
    
    public ID3v1 getId3v1() {
        return new ID3v1(file);
    }
    
    public ID3v2 getId3v2() {
        return new ID3v2(file);
    }
    
    public String getTitre() {
        String titre = id3v2.getInfo("titre");
        
        if (titre.isEmpty())
            titre = id3v1.getTitre();
        
        return titre;
    }
    
    public String getArtiste() {
        String artiste = id3v2.getInfo("artiste");
        
        if (artiste.isEmpty())
            artiste = id3v1.getArtiste();
        
        return artiste;
    }
    
    public String getDuree() {
        WavInfo wi = new WavInfo(file);
        return Mp3Player.nbSecToString(wi.getNbSec());
    }
}
