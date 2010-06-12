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
import java.io.RandomAccessFile;

/**********
 * Classe servant a lire et ecrire les tags ID3v1 des mp3
 * @author Mathieu Lemay
 * @version 1.0
 **********/
public class ID3v1 {
    private File chanson;
    private String titre = "";
    private String artiste = "";
    private String album = "";
    private String annee = "";
    private String commentaire = "";
    private byte piste = 0;
    private int genre = 0;
    private boolean contientTag;
    
    public ID3v1(File chanson) {
        this.chanson = chanson;
        
        try {
            lireTag();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public String getTitre() {
        return titre;
    }
    
    public void setTitre(String titre) {
        this.titre = titre;
    }
    
    public String getArtiste() {
        return artiste;
    }
    
    public void setArtiste(String artiste) {
        this.artiste = artiste;
    }
    
    public String getAlbum() {
        return album;
    }
    
    public void setAlbum(String album) {
        this.album = album;
    }
    
    public String getAnnee() {
        return annee;
    }
    
    public void setAnnee(String annee) {
        this.annee = annee;
    }
    
    public String getCommentaire() {
        return commentaire;
    }
    
    public void setCommentaire(String commentaire) {
        this.commentaire = commentaire;
    }
    
    public byte getPiste() {
        return piste;
    }
    
    public void setPiste(byte piste) {
        this.piste = piste;
    }
    
    public int getGenre() {
        return genre;
    }
    
    public void setGenre(int genre) {
        this.genre = genre;
    }
    
    public void lireTag() throws Exception {
        RandomAccessFile file = new RandomAccessFile(chanson, "r");
        
        //Lire les 128 derniers bits (contiennent l'information)
        int size = (int)file.length();
        file.seek(size - 128);
        byte[] last128 = new byte[128];
        file.read(last128);
        String id3 = new String(last128);
        String tag = id3.substring(0, 3);
        if (tag.equals("TAG")) {
            contientTag = true;
            titre = id3.substring(3, 32).trim();
            artiste = id3.substring(33, 62).trim();
            album = id3.substring(63, 91).trim();
            annee = id3.substring(93, 97).trim();
            commentaire = id3.substring(97, 125).trim();
            //L'octet 125 n'est pas utilise dans la norme ID3v1.1
            piste = last128[126];
            genre = last128[127];
        } else {
            contientTag = false;
        }
        file.close();
    }
    
    public void ecrireTag() {
        byte [] tag = "TAG".getBytes();
        byte [] titre = new byte[30];
        byte [] artiste = new byte[30];
        byte [] album = new byte[30];
        byte [] annee = new byte[4];
        byte [] commentaire = new byte[28];
        byte [] piste = new byte[2];
        byte [] genre = new byte[1];
        
        try {
        	RandomAccessFile file = new RandomAccessFile(chanson, "rws");
            
            if (contientTag)
                file.seek(file.length() - 128);
            else
                file.seek(file.length());
            
            //Padder les elements avec des 0 pour s'assurer qu'ils
            //ont la bonne taille
            titre = arrangerLongueur(this.titre, 30);
            artiste = arrangerLongueur(this.artiste, 30);
            album = arrangerLongueur(this.album, 30);
            annee = arrangerLongueur(this.annee, 4);
            commentaire = arrangerLongueur(this.commentaire, 28);
            //piste[0] = 0, pas besoin d'y toucher
            piste[1] = (byte)this.piste;
            genre[0] = (byte)this.genre;
            
            //Ecriture du tag
            file.write(tag);
            file.write(titre);
            file.write(artiste);
            file.write(album);
            file.write(annee);
            file.write(commentaire);
            file.write(piste);
            file.write(genre);
            
            file.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**********
     * Transformer les donnees en byte[] et les raccourcir ou
     * les padder avec des 0 avant de les ecrire dans le fichier
     * @param s Les donnees
     * @param n La taille desiree des donnees
     * @return Les donnees paddees
     **********/
    public byte[] arrangerLongueur(String s, int n) {
        byte[] temp = new byte[n];
        
        //Raccourcir si trop long
        if (s.length() > n) {
            temp = s.substring(0, n).getBytes();
        } else {
            //Copier b dans temp, sans affecter la longueur de temp;
            for (int i=0; i<s.length(); i++) {
                temp[i] = (byte)s.charAt(i);
            }
            
            //Ajouter des 0;
            for (int i=s.length(); i<n; i++) {
                temp[i] = 0;
            }
        }
        
        return temp;
    }
    
    public void print() {
        if (contientTag) {
            System.out.println("Titre : " + titre);
            System.out.println("Artiste : " + artiste);
            System.out.println("Album : " + album);
            System.out.println("Annee : " + annee);
            System.out.println("Commentaire : " + commentaire);
            System.out.println("Piste : " + piste);
            System.out.println("Genre : " + genre);
        } else {
            System.out.println(chanson.getName() + " ne contiend pas de tag ID3v1");
        }
    }
}
