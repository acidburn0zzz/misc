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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.File;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;

/**********
 * Classe servant a afficher et editer
 * les tags ID3v1 et ID3v2 des mp3
 * @author Mathieu Lemay
 * @version 1.0
 **********/
public class FrameID3Edit extends JDialog {
    private static final long serialVersionUID = 362562375398199536L;
    private File chanson;
    private ID3v1 id3v1;
    private ID3v2 id3v2;
    private Ecouteur e;

    private String titre;
    private String artiste;
    private String album;
    private String annee;
    private String commentaire;
    private String piste;
    private String genre;
    
    private JLabel lblTitre = new JLabel("Titre :");
    private JLabel lblArtiste = new JLabel("Artiste :");
    private JLabel lblAlbum = new JLabel("Album :");
    private JLabel lblAnnee = new JLabel("Annee :");
    private JLabel lblCommentaire = new JLabel("Commentaire :");
    private JLabel lblPiste = new JLabel("Piste :");
    private JLabel lblGenre = new JLabel("Genre :");

    private JTextField txtTitre = new JTextField();
    private JTextField txtArtiste = new JTextField();
    private JTextField txtAlbum = new JTextField();
    private JTextField txtAnnee = new JTextField();
    private JTextField txtCommentaire = new JTextField();
    private JTextField txtPiste = new JTextField();
    private JComboBox cmbGenre = new JComboBox();
    
    private JButton btnCancel = new JButton("Annuler");
    private JButton btnVider = new JButton("Enlever ID3");
    private JButton btnSave = new JButton("Sauvegarder");
    
    public FrameID3Edit(File f) {
        chanson = f;
        try {
            jbInit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public void jbInit() throws Exception {
        id3v1 = new ID3v1(chanson);
        id3v2 = new ID3v2(chanson);
        if (id3v2.isSet()) {
            titre = id3v2.getInfo("titre");
            artiste = id3v2.getInfo("artiste");
            album = id3v2.getInfo("album");
            commentaire = id3v2.getInfo("commentaire");
            annee = id3v2.getInfo("annee");
            piste = id3v2.getInfo("piste");
            genre = id3v2.getInfo("genre");
            if (genre.matches("\\([0-9]+\\)")) {
                int no = Integer.parseInt(genre.substring(1, genre.length()-1));
                genre = ID3Genre.noToString(no);
            } else if (genre.matches("[0-9]+")) {
                int no = Integer.parseInt(genre);
                genre = ID3Genre.noToString(no);
            }
        } else {
            titre = id3v1.getTitre();
            artiste = id3v1.getArtiste();
            album = id3v1.getAlbum();
            commentaire = id3v1.getCommentaire();
            annee = id3v1.getAnnee();
            piste = String.valueOf(id3v1.getPiste());
            genre = ID3Genre.noToString(id3v1.getGenre());
        }
        
        this.setTitle("Proprietes de " + artiste + " - " + titre);
        this.setLayout(null);
        this.setSize(436, 315);
        this.setResizable(false);
        this.setModal(true);
        
        e = new Ecouteur();
        
        //Elements du ComboBox
        String [][] listeGenres = ID3Genre.getListeGenresNom();
        for (int i=0; i<ID3Genre.NB_GENRES; i++) {
            cmbGenre.addItem(listeGenres[i][1]);
        }
                
        //Disposition
        lblTitre.setBounds(10, 10, 100, 25);
        lblArtiste.setBounds(10, 45, 100, 25);
        lblAlbum.setBounds(10, 80, 100, 25);
        lblAnnee.setBounds(10, 115, 100, 25);
        lblCommentaire.setBounds(10, 150, 100, 25);
        lblPiste.setBounds(10, 185, 100, 25);
        lblGenre.setBounds(10, 220, 100, 25);
        txtTitre.setBounds(120, 10, 300, 25);
        txtTitre.setText(titre);
        txtTitre.addCaretListener(e);
        txtArtiste.setBounds(120, 45, 300, 25);
        txtArtiste.setText(artiste);
        txtArtiste.addCaretListener(e);
        txtAlbum.setBounds(120, 80, 300, 25);
        txtAlbum.setText(album);
        txtAnnee.setBounds(120, 115, 300, 25);
        txtAnnee.setText(annee);
        txtCommentaire.setBounds(120, 150, 300, 25);
        txtCommentaire.setText(commentaire);
        txtPiste.setBounds(120, 185, 300, 25);
        txtPiste.setText(String.valueOf(piste));
        cmbGenre.setBounds(120, 220, 300, 25);
        cmbGenre.setEditable(true);
        if (id3v2.isSet())
            cmbGenre.setSelectedItem(genre);
        else {
            cmbGenre.setSelectedItem(ID3Genre.noToString(Integer.parseInt(genre)));
        }
        cmbGenre.addItemListener(e);
        btnCancel.setBounds(25, 255, 125, 25);
        btnCancel.addActionListener(e);
        btnVider.setBounds(160, 255, 125, 25);
        btnVider.addActionListener(e);
        btnSave.setBounds(295, 255, 125, 25);
        btnSave.addActionListener(e);

        this.add(lblTitre);
        this.add(lblArtiste);
        this.add(lblAlbum);
        this.add(lblAnnee);
        this.add(lblCommentaire);
        this.add(lblPiste);
        this.add(lblGenre);
        this.add(txtTitre);
        this.add(txtArtiste);
        this.add(txtAlbum);
        this.add(txtAnnee);
        this.add(txtCommentaire);
        this.add(txtPiste);
        this.add(cmbGenre);
        this.add(btnCancel);
        this.add(btnVider);
        this.add(btnSave);
    }
    
    public void sauvegarder() {
        /**********
         * ID3v1
         **********/
        id3v1.setTitre(txtTitre.getText());
        id3v1.setArtiste(txtArtiste.getText());
        id3v1.setAlbum(txtAlbum.getText());
        id3v1.setAnnee(txtAnnee.getText());
        id3v1.setCommentaire(txtCommentaire.getText());
        
        //No de la piste
        String s = txtPiste.getText();
        byte no;
        if (s.indexOf('/') != -1)
            no = Byte.valueOf(s.substring(0, s.indexOf('/')));
        else
            no = Byte.valueOf(s);
        if (no > 99)
            no = 99;
        id3v1.setPiste(no);
        
        //Genre
        int noGenre = ID3Genre.stringToNo((String)cmbGenre.getSelectedItem());
        if (noGenre == -1)
            noGenre = ID3Genre.stringToNo("Other");
        
        id3v1.setGenre(noGenre);
        
        id3v1.ecrireTag();
        
        /**********
         * ID3v2
         **********/
        id3v2.setInfo("titre", txtTitre.getText());
        id3v2.setInfo("artiste", txtArtiste.getText());
        id3v2.setInfo("album", txtAlbum.getText());
        id3v2.setInfo("annee", txtAnnee.getText());
        id3v2.setInfo("commentaire", txtCommentaire.getText());
        id3v2.setInfo("piste", txtPiste.getText());
        
        String genre;
        noGenre = ID3Genre.stringToNo((String)cmbGenre.getSelectedItem());
        if (noGenre == -1)
            genre = (String)cmbGenre.getSelectedItem();
        else
            genre = "(" + String.valueOf(noGenre) + ")";
        
        id3v2.setInfo("genre", genre);
        
        id3v2.ecrireTag();
        
        dispose();
    }
    
    private class Ecouteur implements ActionListener, CaretListener, ItemListener {
        public void actionPerformed(ActionEvent e) {
            if (e.getSource() == btnCancel) {
                dispose();
            } else if (e.getSource() == btnVider) {
                txtTitre.setText("");
                txtArtiste.setText("");
                txtAlbum.setText("");
                txtAnnee.setText("");
                txtCommentaire.setText("");
                txtPiste.setText("");
            } else if (e.getSource() == btnSave) {
                sauvegarder();
            }
        }

        public void itemStateChanged(ItemEvent e) {
            genre = (String)cmbGenre.getSelectedItem();
        }

        public void caretUpdate(CaretEvent e) {
            if (e.getSource() == txtTitre || e.getSource() == txtArtiste) {
                setTitle("Proprietes de " + txtArtiste.getText() + " - " + txtTitre.getText());
            }
        }
    }
}
