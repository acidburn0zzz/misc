package org.acidrain.player;

import java.io.IOException;
import java.io.Serializable;

import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.BooleanControl;
import javax.sound.sampled.DataLine;
import javax.sound.sampled.FloatControl;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.SourceDataLine;

@SuppressWarnings("serial")
public class WavDiffuseur extends Thread implements Serializable {
    private static final int GRANDEUR_BUFFER = 16000; // grandeur du buffer 12800

    public static final int PLAY = 0;
    public static final int PAUSE = 1;
    public static final int STOP = 2;

    private int statut=PLAY;  // valeurs possibles 
    private WavInfo w;
    private SourceDataLine ligne = null; // ligne communiquant avec la carte de son
    
    private int nBytesWritten = 0;
    private int volume;

    public WavDiffuseur(WavInfo w, int volume) {
        this.w = w;
        this.volume = volume;
    }

    public WavInfo getWavInfo() {
        return w;
    }

    public void run() { // methode appelee automatiquement lorsqu'on demarre un thread
        // Nous devons obtenir une ligne ( Line ) afin de disposer de notre fichier lu; la line fait le lien entre notre programme et la carte de son de l'ordinateur.
        // Nous avons choisi une SourceDataLine car nous voulons ecouter et non pas enregistrer de la musique
        // l'obtention de la ligne necessite l'audioFormat car on a besoin de savoir quel format les donnees passees a la ligne auront.

        DataLine.Info info = new DataLine.Info(SourceDataLine.class, w.getAudioFormatDecode());
        try {
            ligne = (SourceDataLine) AudioSystem.getLine(info);
            /*La line est creee nous devons l'ouvrir pour qu'elle puisse recevoir des donnees audio*/
            ligne.open(w.getAudioFormatDecode());
        } catch (LineUnavailableException e) {
            e.printStackTrace();
            System.exit(1);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }

        /*Ce n'est pas encore assez. La line doit recevoir des donnees, mais ne peut pas les passer a la carte audio de l'ordi, il faut l'activer*/
        ligne.start();
        /*On regle le volume selon ce qui a ete passe au constructeur*/
        setVolume(volume);
        /*
        Maintenant que la ligne est preparee, on doit ecrire les donnees a cette ligne grace a une boucle.  On lit les donnees du stream a un buffer
        et ensuite on ecrit les donnees du buffer a la line. Quand la methode read retourne -1 , cela veut dire que tout le fichier a ete lu.
        */
        int nBytesRead = 0;
        byte[] abData = new byte[GRANDEUR_BUFFER];

        //synchronized
        synchronized (w.getAudioInputStreamDecode()) {
        while ((nBytesRead != -1) && (statut != STOP)) { 
            if (statut == PLAY) { // je peux lire
                try {
                    nBytesRead = w.getAudioInputStreamDecode().read(abData, 0, abData.length);  
                    if (nBytesRead >= 0) { // on a eu des octets lus en banque
                        nBytesWritten += ligne.write(abData, 0, nBytesRead);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else { //if ( statut == PAUSE )// je veux faire pause
                try {
                    Thread.sleep(100); // le thread est en sommeil, le sera tant qu'on remet pas le statut a play
                } catch (InterruptedException ie) {
                    ie.printStackTrace();
                }
            }
        } // fin du while

        /*Toutes les donnees ont ete lues et jouees ( ou on a appuyer sur stop ), on peut fermer la line*/
        statut = STOP;
        ligne.drain();
        ligne.close(); // on ferme la ligne , lecture terminee
        //w.getAudioInputStreamDecode().close()
        }
    }

    public void setLigne(SourceDataLine ligne) {
        this.ligne = ligne;
    }

    public SourceDataLine getLigne() {
        return ligne;
    }

    public void setStatut(int statut) {
        this.statut = statut;
    }

    public int getStatut() {
        return statut;
    }
    
    public void setVolume(int volume) {
        /*Pour s'assurer que le volume est entre 0 et 100*/
        if (volume > 100) {
            volume = 100;
        } else if (volume < 0) {
            volume = 0;
        }
        
        this.volume = volume;
        
        FloatControl gainControl = (FloatControl)ligne.getControl(FloatControl.Type.MASTER_GAIN);
        double gain = volume/100.0;
        float dB = (float)(Math.log(gain)/Math.log(10.0)*20.0);
        gainControl.setValue(dB);
    }
    
    public int getVolume() {
        return volume;
    }
    
    public void setMute(boolean mute) {
        BooleanControl muteControl = (BooleanControl)ligne.getControl(BooleanControl.Type.MUTE);
        muteControl.setValue(mute);
    }
    
    public int getNbSecJouees() {
        //Au cas ou la ligne ne soit pas prete encore
        try {
            return (int)(ligne.getMicrosecondPosition() / 1000000.0);
        } catch (Exception e) {
            e.printStackTrace();
        }
        
        //Retourne 0 si la ligne n'est pas prete
        return 0;
    }
    
    public int getNbSecRestantes() {
        return w.getNbSec() - getNbSecJouees();
    }
}
