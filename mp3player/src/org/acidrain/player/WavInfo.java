package org.acidrain.player;

import javax.sound.sampled.*;
import javax.swing.JOptionPane;

import java.io.*;

@SuppressWarnings("serial")
public class WavInfo implements Serializable {
    private File fichier; // nom de la chanson a jouer
    private AudioInputStream audioInputStream = null; // on lira le fichier musical a partir d'un stream AudioInputStream, on lira les octets du fichier a partir de ce stream
    private AudioInputStream audioInputStreamDecode = null; // stream supplementaire necessaire pour lire des mp3 ( encodage ) 
    private AudioFormat audioFormat;
    private AudioFormat audioFormatDecode;
    private int nbSec; // nb de secondes que dure la chanson


    public WavInfo(File fichier) {
        try {
            this.fichier = fichier;
            audioInputStream = AudioSystem.getAudioInputStream(fichier); // obtient un audioInputStream a partir d'un fichier audio VALIDE
            audioFormat = audioInputStream.getFormat(); // on va chercher le format de notre stream ( notre fichier son ) comme la frequence, on aura besoin de ces infos pour obtenir une ligne d'output
            audioFormatDecode = new AudioFormat(
            AudioFormat.Encoding.PCM_SIGNED /*technique d'encodage audio */,
            audioFormat.getSampleRate()/* nbre de samples par seconde*/, 16/* nbre de bits ds chaque sample*/, audioFormat.getChannels()/* canaux a utiliser*/,
            audioFormat.getChannels() * 2/*nbre de bits ds chaque frame */, audioFormat.getSampleRate()/* nbre de frames par seconde*/,
            false);
            audioInputStreamDecode = AudioSystem.getAudioInputStream(audioFormatDecode, audioInputStream);//conversion ds le format Decode ( mp3 ) de l'inputStream initial
            
            double oct;
            if (audioFormat.properties().get("bitrate") == null) {
            	oct = audioFormat.getFrameRate() * audioFormat.getFrameSize();
            } else {
            	oct = (Integer)audioFormat.properties().get("bitrate")/8.0; // nb d'octets de la chanson par seconde ( 8 bits ds un octet )
            }
            long longueur = fichier.length(); // longueur du fichier en octets

            nbSec =  (int) (longueur / oct); // nb d'octets total divise par le nb d'octets par seconde = duree de la chanson
        } catch (IllegalArgumentException e) {
            JOptionPane.showMessageDialog(null, "Le fichier choisi n'est pas valide", "Fichier invalide", JOptionPane.ERROR_MESSAGE);
        } catch (UnsupportedAudioFileException e) {
            JOptionPane.showMessageDialog(null, "Le fichier choisi n'est pas valide", "Fichier invalide", JOptionPane.ERROR_MESSAGE);
        } catch (Exception e) {
            /*si ca ne marche pas*/
            e.printStackTrace();
            System.exit(1);
        }
    }

    public File getFichier() {
        return fichier;
    }

    public AudioInputStream getAudioInputStream() {
        return audioInputStream;
    }

    public AudioFormat getAudioFormat() {
        return audioFormat;
    }

    public  int getNbSec() {
        return nbSec;
    }

    public boolean equals(Object  o) {
        WavInfo w = ( WavInfo ) o;
        if (w.getFichier().equals(this.getFichier()))  {
            if (w.getNbSec() == this.getNbSec()) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    public void setAudioFormatDecode(AudioFormat audioFormatDecode) {
        this.audioFormatDecode = audioFormatDecode;
    }

    public AudioFormat getAudioFormatDecode() {
        return audioFormatDecode;
    }

    public void setAudioInputStreamDecode(AudioInputStream audioInputStreamDecode) {
        this.audioInputStreamDecode = audioInputStreamDecode;
    }

    public AudioInputStream getAudioInputStreamDecode() {
        return audioInputStreamDecode;
    }
}
