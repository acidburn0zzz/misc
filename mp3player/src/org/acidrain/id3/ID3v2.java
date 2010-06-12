/****************************************************************************
 * Copyright (C) 2007 Lemay, Mathieu                                        *                             
 *                                                                          *
 * This program is free software; you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation; either version 2 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License along  *
 * with this program; if not, write to the Free Software Foundation, Inc.,  *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.              *
 *                                                                          *
 * You can contact the original author at acidrain1@gmail.com               *
 ****************************************************************************/

package org.acidrain.id3;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.RandomAccessFile;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

/**********
 * Classe servant a lire et ecrire les tags ID3v2 des mp3
 * @author Mathieu Lemay
 * @version 0.1
 **********/
public class ID3v2 {
    private File chanson;
    
    /**********
     * Informations sur le tag
     **********/
    //Informations sur le tag
    private boolean contientTag;
    private byte id3VersionMajor;
    private byte id3VersionMinor;
    private int tailleTotaleTag;
    private int tailleTag;
    private int tailleHeaderFooter;
    private int tailleFrames;
    
    //Header Flags
    private boolean unsynchronisation = false;
    private boolean extendedHeader = false;
    private boolean experimental = false;
    private boolean footerPresent = false;
    
    //Extended Header
    private int extendedHeaderSize = 0;
    private int paddingSize = 0;
    private int crc32 = 0;
    
    /**********
     * Informations contenues dans les frames
     **********/
    private Hashtable<String, String> infosPrincipales = new Hashtable<String, String>();
    //Vecteur car il peut y avoir plus d'un frame avec le meme id
    private Vector<String[]> infosSecondaires = new Vector<String[]>();
    
    //Extended Header Flags
    private boolean crc32Present = false;
    
    public ID3v2(File f) {
        chanson = f;
        
        //Initialiser les infosPrincipales avec des valeurs null
        //Au cas ou des informations ne sont pas dans le tag
        infosPrincipales.put("titre", "");
        infosPrincipales.put("album", "");
        infosPrincipales.put("annee", "");
        infosPrincipales.put("artiste", "");
        infosPrincipales.put("piste", "");
        infosPrincipales.put("genre", "");
        infosPrincipales.put("commentaire", "");
        infosPrincipales.put("noDisque", "");
        infosPrincipales.put("compositeur", "");
        infosPrincipales.put("artisteOriginal", "");
        infosPrincipales.put("copyright", "");
        infosPrincipales.put("url", "");
        infosPrincipales.put("infoSuppl", "");
        
        try {
            contientTag = lireTag();
        } catch (Exception e) {
            e.printStackTrace();
            contientTag = false;
        }
    }
    
    public String getInfo(String key) {
        return infosPrincipales.get(key);
    }
    
    public void setInfo(String key, String value) {
        infosPrincipales.put(key, value);
    }
    
    public boolean isSet() {
        return contientTag;
    }
    
    public boolean lireTag() throws Exception {
        FileInputStream stream = new FileInputStream(chanson);
        int dataSize;
        byte[] data;
        int bytesRead = 0;
        
        /**********
         * HEADER
         **********/
        dataSize = 10;
        data = new byte[dataSize];
        bytesRead += stream.read(data);
        
        String header = new String(data);
        if (!header.substring(0, 3).equals("ID3") || data[3] == 0xFF || data[4] == 0xFF
                || data[6] >= 0x80 || data[7] >= 0x80 || data[8] >= 0x80 || data[9] >= 0x80) {

            stream.close();
            return false;
        }
        
        //ID3 Version
        id3VersionMajor = data[3];
        id3VersionMinor = data[4];
        
        //Flags (sous la forme %abcd0000)
        if ((data[5] & 0x80) != 0)
            unsynchronisation = true;
        if ((data[5] & 0x40) != 0)
            extendedHeader = true;
        if ((data[5] & 0x20) != 0)
            experimental = true;
        if ((data[5] & 0x10) != 0)
            footerPresent = true;
        
        //Taille du tag
        tailleTag = data[6] << 21 | data[7] << 14 | data[8] << 7 | data[9];
        
        //Taille de Header + Footer
        if (footerPresent)
            tailleHeaderFooter = 20;
        else
            tailleHeaderFooter = 10;
        tailleTotaleTag = tailleTag + tailleHeaderFooter; //Ajouter la taille du Header/Footer
        
        /**********
         * EXTENDED HEADER ID3v2.3.0
         **********/
        if (extendedHeader && id3VersionMajor == 3) {
            //Taille du extenderHeader
            dataSize = 4;
            data = new byte[dataSize];
            bytesRead += stream.read(data);
            extendedHeaderSize = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
            extendedHeaderSize += 4; //Ajouter les 4 octets qui contiennent la taille
            
            //Reste du header (6 ou 10 bytes)
            dataSize = extendedHeaderSize - 4;
            data = new byte[dataSize];
            bytesRead += stream.read(data);
            
            //Flags
            if ((data[0] & 0x80) != 0)
                crc32Present = true;
            
            paddingSize = data[2] << 24 | data[3] << 16 | data[4] << 8 | data[5];
            
            if (crc32Present)
                crc32 = data[6] << 24 | data[7] << 16 | data[8] << 8 | data[9];
        }
        
        /**********
         * EXTENDED HEADER ID3v2.4.0
         **********/
        if (extendedHeader && id3VersionMajor == 4) {
            //TODO
        }
        
        /**********
         * FRAMES
         **********/
        int bytesRestantsFrame = tailleTag - extendedHeaderSize - paddingSize;
        while (bytesRestantsFrame > 0) {
            //Informations sur le frame
            String frameHeader;
            String frameID;
            //String frameData;
            int frameSize = 0;
            
            //Flags du frame
            
            //Header du frame
            dataSize = 10;
            data = new byte[dataSize];
            bytesRead += stream.read(data);
            
            frameHeader = new String(data);
            frameID = frameHeader.substring(0, 4);
            if (frameID.charAt(0) == (char)0) {//Si le premier char du frameID est null, on est dans du padding
                bytesRestantsFrame = 0;
                continue;
            }
            
            if (id3VersionMajor == 3)
                frameSize = data[4] << 24 | data[5] << 16 | data[6] << 8 | data[7];
            if (id3VersionMajor == 4)
                frameSize = data[4] << 21 | data[5] << 14 | data[6] << 7 | data[7];
            
            //TODO Flags du frame (data[8] et data[9])            
            
            //Data du frame
            dataSize = frameSize;
            //Au cas ou il y aurait un erreur
            /*if (dataSize < 0)
            	break;*/
            data = new byte[dataSize];
            bytesRead += stream.read(data);

            processData(frameID, data);
            
            bytesRestantsFrame -= 10;  //Enlever la taille du header
            bytesRestantsFrame -= frameSize;
            
            /*Ajouter la taille de ce frame a la taille totale des frames
              Sert quand il y a du padding*/
            tailleFrames += 10 + frameSize;
        }
        
        //Si il y a du padding mais ce n'etait pas indique dans le header
        if (paddingSize == 0)
            paddingSize = tailleTag - extendedHeaderSize - tailleFrames;
        
        stream.close();
        return true;
    }
    
    public void ecrireTag() {
        byte[][][] donnees;             //Donnees a ecrire
        byte[] header = new byte[10];   //Header du tag
        int nbFrames = 0;               //Nombre de frames a ecrire
        int noFrame = 0;                //Position du frame dans donnees
        int tagSize = 0;                //Taille du tag, sans le header
        int nbBytesEcrits = 0;          //Compteur du nombre de bytes ecrits

        int bufferSize;                 //Taille du buffer pour le padding
        byte[] buffer;                  //Buffer pour le padding
        
        /**********
         * Creation des bytes array de donnees
         **********/
        //Calcul du nombre de frames
        for (Enumeration<String> en = infosPrincipales.keys(); en.hasMoreElements();)
            if (!infosPrincipales.get(en.nextElement()).equals(""))
                nbFrames++;
        //nbFrames += infosSecondaires.size();
        
        //Initialisation du array de donnees
        donnees = new byte[nbFrames][2][];
        
        //Ajout des donnees au array
        for (Enumeration<String> en = infosPrincipales.keys(); en.hasMoreElements();) {
            String key = en.nextElement();
            if (!infosPrincipales.get(key).equals("")) {
                donnees[noFrame] = createFrame(key, infosPrincipales.get(key));
                noFrame++;
            }
        }
        
        /**********
         * Header du ID3
         **********/
        //Calcul de la taille des frames
        for (int i=0; i<nbFrames; i++) {
            tagSize += donnees[i][0].length + donnees[i][1].length;
        }
        
        //Si le tag est plus petit que le precedent
        //on garde le padding
        if (tagSize + 10 < tailleTotaleTag)
            tagSize = tailleTotaleTag-10;
        else //On arrondit au Ko suivant moins 10octets pour le header
            tagSize += ((1024 - tagSize%1024));
        
        //Header
        header[0] = 'I';    header[1] = 'D';    header[2] = '3';                //ID3
        header[3] = (byte)4;    header[4] = (byte)0;    header[5] = (byte)0;    //Version 2.4.0 + Flags
        header[6] = (byte)((tagSize >> 21) & 0x7F);                             //Taille du tag
        header[7] = (byte)((tagSize >> 14) & 0x7F);
        header[8] = (byte)((tagSize >> 7) & 0x7F);
        header[9] = (byte)(tagSize & 0x7F);
        
        /**********
         * Ecriture dans le fichier
         **********/
        try {
            //Verification si il y a assez de place pour ecrire le tag
            if (tagSize + 10 <= tailleTotaleTag) { //On a de la place
                RandomAccessFile file = new RandomAccessFile(chanson, "rws");
                
                //Header
                file.write(header);  //Header
                nbBytesEcrits += header.length;
                
                //Ecriture des frames
                for (int i=0; i<nbFrames; i++) {
                    file.write(donnees[i][0]);   //Header
                    nbBytesEcrits += donnees[i][0].length;
                    file.write(donnees[i][1]);   //Data
                    nbBytesEcrits += donnees[i][1].length;
                }
                
                //Padding
                while (nbBytesEcrits < tagSize) {
                    bufferSize = tagSize - nbBytesEcrits;
                    buffer = new byte[bufferSize];
                    file.write(buffer);
                    nbBytesEcrits += bufferSize;
                }
                
                file.close();
            } else {
                //On doit re-ecrire le fichier
                File temp;
                if (System.getProperty("os.name").toLowerCase().contains("windows"))
                    temp = new File(chanson.getParent() + "\temp.mp3");
                else
                    temp = new File(chanson.getParent() + "/temp.mp3");
                
                FileInputStream fis = new FileInputStream(chanson);
                FileOutputStream fos = new FileOutputStream(temp);
                bufferSize = 16384;
                buffer = new byte[bufferSize];
                
                //Header
                fos.write(header);  //Header
                nbBytesEcrits += header.length;
                
                //Ecriture des frames
                for (int i=0; i<nbFrames; i++) {
                    fos.write(donnees[i][0]);   //Header
                    nbBytesEcrits += donnees[i][0].length;
                    fos.write(donnees[i][1]);   //Data
                    nbBytesEcrits += donnees[i][1].length;
                }
                
                //Padding
                while (nbBytesEcrits < tagSize) {
                    bufferSize = tagSize - nbBytesEcrits;
                    buffer = new byte[bufferSize];
                    fos.write(buffer);
                    nbBytesEcrits += bufferSize;
                }
                
                //Copier les donnees de l'ancien fichier
                fis.skip((long)tailleTotaleTag);
                
                while (fis.read(buffer) > 0) {
                    fos.write(buffer);
                    nbBytesEcrits += bufferSize;
                }
                
                fis.close();
                fos.close();
                
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**********
     * Trie les donnees des frames
     * @param frameID ID du frame
     * @param frameData Valeur du frame
     * @return True si la donnee va dans infosPrincipales, False sinon
     **********/
    public boolean processData(String frameID, byte[] frameData) {
        String key = "";
        String data;
        
        if (frameID.equals("TIT2")) { //Titre de la chanson
            key = "titre";
        } else if (frameID.equals("TPE1")) { //Artiste principal
            key = "artiste";
        } else if (frameID.equals("TALB")) { //Album
            key = "album";
        } else if (frameID.equals("TYER")) { //Annee
            key = "annee";
        } else if (frameID.equals("TRCK")) { //Piste
            key = "piste";
        } else if (frameID.equals("TCON")) { //Genre
            key = "genre";
        } else if (frameID.equals("COMM")) { //Commentaire
            /*Les 5 premiers bytes sont pour la description du Commentaire
             * Byte 0 = Encoding
             * Byte 1-3 = Language
             * Byte 4 = Short content descrip.
             * TODO S'occuper du commDescript et du Encoding
             */
            byte encoding = frameData[0];
            String language = new String(frameData, 1, 3);
            byte shortDescript = frameData[4];
            String commentaire = new String(frameData, 5, frameData.length-5/*, Charset charset*/);
            
            infosPrincipales.put("commentaire", commentaire);
            return true;
        } else if (frameID.equals("TPOS")) { //# du disque
            key = "noDisque";
        } else if (frameID.equals("TCOM")) { //Compositeur
            key = "compositeur";
        } else if (frameID.equals("TOPE")) { //Artiste original
            key = "artisteOriginal";
        } else if (frameID.equals("TCOP")) { //Copyright
            key = "copyright";
        } else if (frameID.equals("WXXX")) { //URL
            key = "url";
        } else if (frameID.equals("TXXX")) { //Info suppl.
            key = "infoSuppl";
        } else { //Ajouter les autres cles sans alteration dans infoSecondaires
            infosSecondaires.add(new String[] {frameID, new String(frameData)});
            return false;
        }
        
        //Remplacer les caracteres null par des espaces
        data = new String(frameData).replace((char)0, ' ');
        //Enlever les espaces au debut et a la fin
        data = data.trim();
        
        /**********
         * Mesure temporaire pour enlever
         * les caracteres invalides
         **********/
        /*
        data = new String(frameData);
        String temp = "";
        
        for (int i=0; i<data.length(); i++) {
        	if (ID3Genre.validChars.indexOf(data.charAt(i)) != -1)
        		temp += data.charAt(i);
        }
        
        data = temp;
        */
        
        infosPrincipales.put(key, data);
        return true;
    }
    
    /**********
     * Cree un frame en fonction du type d'information et des donnees
     * @param key Type d'info
     * @param data Valeur du frame
     * @return Un byte[][] contenant le frame
     **********/
    public byte[][] createFrame(String key, String data) {
        byte[][] frame = new byte[2][];
        byte[] header = null;
        byte[] taille = new byte[4];
        byte[] value;
        
        value = data.getBytes();
        
        //Calcul de la taille du frame
        int i = value.length;
        taille[0] = (byte)((i >> 21) & 0x7F);
        taille[1] = (byte)((i >> 14) & 0x7F);
        taille[2] = (byte)((i >> 7) & 0x7F);
        taille[3] = (byte)(i & 0x7F);
        
        //Creation du header en fonction du type de donnees
        if (key.equals("titre")) {
            header = new byte[] {'T', 'I', 'T', '2', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("artiste")) {
            header = new byte[] {'T', 'P', 'E', '1', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("album")) {
            header = new byte[] {'T', 'A', 'L', 'B', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("annee")) {
            header = new byte[] {'T', 'Y', 'E', 'R', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("piste")) {
            header = new byte[] {'T', 'R', 'C', 'K', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("genre")) {
            header = new byte[] {'T', 'C', 'O', 'N', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("commentaire")) {
            header = new byte[] {'C', 'O', 'M', 'M', taille[0], taille[1], taille[2], taille[3], 0, 0};
            //Ajout de la description du commentaire
            byte[] descript = {03, 'X', 'X', 'X', 0};   //03 pour le premier octet, parce que c'est ca que j'ai vu dans mes mp3
            byte[] temp = new byte[value.length + 5];
            System.arraycopy(descript, 0, temp, 0, 5);
            System.arraycopy(value, 0, temp, 5, value.length);
            value = temp;
            
            //Recalculer la taille
            i = value.length+5; //+5 a cause de la description
            header[4] = (byte)((i >> 21) & 0x7F);
            header[5] = (byte)((i >> 14) & 0x7F);
            header[6] = (byte)((i >> 7) & 0x7F);
            header[7] = (byte)(i & 0x7F);
        } else if (key.equals("noDisque")) {
            header = new byte[] {'T', 'P', 'O', 'S', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("compositeur")) {
            header = new byte[] {'T', 'C', 'O', 'M', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("artisteOriginal")) {
            header = new byte[] {'T', 'O', 'P', 'E', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("copyright")) {
            header = new byte[] {'T', 'C', 'O', 'P', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("url")) {
            header = new byte[] {'W', 'X', 'X', 'X', taille[0], taille[1], taille[2], taille[3], 0, 0};
        } else if (key.equals("infoSuppl")) {
            header = new byte[] {'T', 'X', 'X', 'X', taille[0], taille[1], taille[2], taille[3], 0, 0};
        }
        
        frame[0] = header;
        frame[1] = value;
        
        return frame;
    }
    
    public void printTag() {
        if (contientTag) {
            System.out.println("ID3v2." + id3VersionMajor + "." + id3VersionMinor);
            System.out.println("Taille du tag : 0x" + Integer.toHexString(tailleTag));
            System.out.println("Taille du tag (inc. headers) : 0x" + Integer.toHexString(tailleTotaleTag));
            System.out.println("Taille du Extended header : 0x" + Integer.toHexString(extendedHeaderSize));
            System.out.println("Taille du Padding : 0x" + Integer.toHexString(paddingSize));
            System.out.println("Taille des Frames : 0x" + Integer.toHexString(tailleFrames));
            if (tailleFrames + extendedHeaderSize + paddingSize + tailleHeaderFooter == tailleTotaleTag)
                System.out.println("Tailles OK");
            
            for (Enumeration<String> en = infosPrincipales.keys(); en.hasMoreElements(); ) {
                String s = en.nextElement();
                System.out.println(s + " = " + infosPrincipales.get(s));
            }
        } else {
            System.out.println("Le fichier " + chanson.getName() + " ne contient pas de tag ID3v2");
        }
    }
}
