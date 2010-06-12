package org.acidrain.player;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.table.DefaultTableModel;

import org.acidrain.id3.FrameID3Edit;


@SuppressWarnings("serial")
public class Playlist extends JPanel {
    private Mp3Player player;
    private Vector<Chanson> chansons = new Vector<Chanson>();
    private String nomPlaylist;
    private boolean shuffle;
    private boolean repeat;
    private int noChansonJouee = -1; //Pour qu'il n'y ait aucune chanson en gras au debut
    private int noChansonSelectionnee;
    private Ecouteur e;
    
    private JPopupMenu popMenu = new JPopupMenu();
    private JMenuItem itmPlay = new JMenuItem("Play");
    private JMenuItem itmAddChanson = new JMenuItem("Ajouter une chanson");
    private JMenuItem itmRemChanson = new JMenuItem("Enlever la chanson");
    private JMenuItem itmRenChanson = new JMenuItem("Renommer la chanson");
    private JMenuItem itmMoveUp = new JMenuItem("Deplacer vers le haut");
    private JMenuItem itmMoveDown = new JMenuItem("Deplacer vers le bas");
    private JMenuItem itmAddPLaylist = new JMenuItem("Ajouter une playlist");
    private JMenuItem itmRemPLaylist = new JMenuItem("Enlever la playlist");
    private JMenuItem itmRenPlaylist = new JMenuItem("Renommer la playlist");
    private JMenuItem itmImpPlaylist = new JMenuItem("Importer une playlist");
    private JMenuItem itmExpPlaylist = new JMenuItem("Exporter la playlist");

    private DefaultTableModel listeChansons = new DefaultTableModel(new String[] {"no", "titre", "duree"}, 0);
    private JTable tableChansons = new JTable(listeChansons);
    private PlaylistCellEditor cellEditor = new PlaylistCellEditor(new JTextField());
    private JScrollPane panelListe = new JScrollPane(tableChansons);
    
    private JButton btnAddChanson = new JButton("+");
    private JButton btnRemChanson = new JButton("-");
    private JButton btnAddPlaylist = new JButton("++");
    private JButton btnRemPlaylist = new JButton("--");
    private JComboBox cmbListes = new JComboBox();
    
    //Je me base sur la font par defaut d'un bouton pour en faire une plus petite
    private Font fontBoutons = new Font(btnAddChanson.getFont().getFontName(), Font.PLAIN, btnAddChanson.getFont().getSize()-4);
    
    public Playlist(Mp3Player player, String nom, boolean shuffle, boolean repeat) {
        this.player = player;
        this.nomPlaylist = nom;
        this.shuffle = shuffle;
        this.repeat = repeat;
        
        try {
            jbInit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public void jbInit() throws Exception {
        this.setSize(383, 300);
        this.setLayout(null);
        
        e = new Ecouteur();
        
        //Proprietes de la table
        tableChansons.setTableHeader(null);
        tableChansons.getColumn("no").setMinWidth(30);
        tableChansons.getColumn("no").setMaxWidth(30);
        tableChansons.getColumn("duree").setMinWidth(40);
        tableChansons.getColumn("duree").setMaxWidth(40);
        tableChansons.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        tableChansons.setDefaultRenderer(tableChansons.getColumnClass(0), new PlaylistCellRenderer(this));
        tableChansons.setDefaultEditor(tableChansons.getColumnClass(0), cellEditor);
        tableChansons.setShowGrid(false);
        tableChansons.setIntercellSpacing(new Dimension(0, 0));
        tableChansons.addKeyListener(e);
        tableChansons.addMouseListener(e);
        tableChansons.addMouseMotionListener(e);
        
        //Disposition
        panelListe.setBounds(0, 0, 373, 255);
        panelListe.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        btnAddChanson.setBounds(0, 265, 50, 25);
        btnAddChanson.setFont(fontBoutons);
        btnAddChanson.addActionListener(e);
        btnRemChanson.setBounds(50, 265, 50, 25);
        btnRemChanson.setFont(fontBoutons);
        btnRemChanson.addActionListener(e);
        btnAddPlaylist.setBounds(125, 265, 50, 25);
        btnAddPlaylist.setFont(fontBoutons);
        btnAddPlaylist.addActionListener(e);
        btnRemPlaylist.setBounds(175, 265, 50, 25);
        btnRemPlaylist.setFont(fontBoutons);
        btnRemPlaylist.addActionListener(e);
        cmbListes.setBounds(250, 265, 123, 25);
        
        this.add(panelListe);
        this.add(btnAddChanson);
        this.add(btnRemChanson);
        this.add(btnAddPlaylist);
        this.add(btnRemPlaylist);
        this.add(cmbListes);
        
        //PopupMenu
        popMenu.add(itmPlay);
        itmPlay.addActionListener(e);
        popMenu.addSeparator();
        popMenu.add(itmAddChanson);
        itmAddChanson.addActionListener(e);
        popMenu.add(itmRemChanson);
        itmRemChanson.addActionListener(e);
        popMenu.add(itmRenChanson);
        itmRenChanson.addActionListener(e);
        popMenu.add(itmMoveUp);
        itmMoveUp.addActionListener(e);
        popMenu.add(itmMoveDown);
        itmMoveDown.addActionListener(e);
        popMenu.addSeparator();
        popMenu.add(itmAddPLaylist);
        itmAddPLaylist.addActionListener(e);
        popMenu.add(itmRemPLaylist);
        itmRemPLaylist.addActionListener(e);
        popMenu.add(itmRenPlaylist);
        itmRenPlaylist.addActionListener(e);
        popMenu.addSeparator();
        popMenu.add(itmImpPlaylist);
        itmImpPlaylist.addActionListener(e);
        popMenu.add(itmExpPlaylist);
        itmExpPlaylist.addActionListener(e);
    }
    
    /********
     * Met a jour le ComboBox
     ********/
    public void createComboBox() {
        //J'enleve le listener pour ne pas creer d'evenements durant la creation
        cmbListes.removeActionListener(e);
        //Comme la fonction est appelee 3 fois par clic dans le combo box
        //je dois vider le combo box chaque fois pour ne pas avoir de doubles
        cmbListes.removeAllItems();
        
        Hashtable<String, Playlist> temp = player.getListes();
        Enumeration<String> en;
        for (en = temp.keys(); en.hasMoreElements();) {
            cmbListes.addItem(en.nextElement());
        }
        
        cmbListes.setSelectedItem(nomPlaylist);
        //J'ajoute l'action listener apres pour ne pas creer
        //d'evenements pendant la creation
        cmbListes.addActionListener(e);
    }
    
    /********
     * Retourne le File de la chanson en cours
     * Est appelee quand on change de chanson
     * 
     * @return File de la chanson en cours ou null si la playlist est vide
     ********/
    public Chanson getChanson() {
        //Redessiner la table pour s'assurer qu'il n'y a qu'une case en gras
        tableChansons.repaint();
        
        if (chansons.size() > 0) {
            return chansons.get(noChansonJouee);
        } else {
            return null;
        }
    }
    
    /********
     * Retourne le numero de la chanson qui joue en ce moment
     * 
     * @return No de la chanson
     ********/
    public int getNoChansonJouee() {
        return noChansonJouee;
    }
    
    /********
     * Definit quelle chanson sera celle qui va jouer
     * 
     * @param noChansonJouee Le no de la chanson qui sera jouee
     ********/
    public void setNoChansonJouee(int noChansonJouee) {
        this.noChansonJouee = noChansonJouee;
    }
    
    /********
     * Retourne le nom de la chanson dans la liste
     * Il est possible de changer le nom dans la table
     * 
     * @return Nom de toutes les chansons dans un vecteur
     ********/
    public Vector<String> getTousNomsChansons() {
        Vector<String> tousNomsChansons = new Vector<String>();
        
        for (int i=0; i<getNbChansons(); i++) {
            tousNomsChansons.add((String)listeChansons.getValueAt(i, 1));
        }
        
        return tousNomsChansons;
    }
    
    /********
     * Ajoute tous les noms de chansons dans la table
     * 
     * @param tousNomsChansons Vecteur contenant la liste des noms
     ********/
    public void setTousNomsChansons(Vector<String> tousNomsChansons) {
        for (int i=0; i<tousNomsChansons.size(); i++) {
            listeChansons.setValueAt((String)tousNomsChansons.get(i), i, 1);
        }
    }
    
    /********
     * Retourne le nombre de chansons dans la playlist
     * 
     * @return Nbre de chansons
     ********/
    public int getNbChansons() {
        return chansons.size();
    }
    
    /********
     * Retourne le nom de la playlist
     * 
     * @return Nom de la playlist
     ********/
    public String getNomPlaylist() {
        return nomPlaylist;
    }
    
    /********
     * Change le nom de cette playlist
     * 
     * @param nom Le nouveau nom
     ********/
    public void setNomPlaylist(String nom) {
        this.nomPlaylist = nom;
    }
    
    /********
     * Retourne la liste des fichiers mp3
     * 
     * @return Liste des fichiers
     ********/
    public Vector<File> getChansons() {
    	Vector<File> listeChansons = new Vector<File>(chansons.size(), 0);
    	for (int i=0; i<chansons.size(); i++)
    		listeChansons.add(chansons.get(i).getFile());
        return listeChansons;
    }
    
    /********
     * Definit si la playlist jouera en ordre ou non
     * 
     * @param b Valeur du shuffle
     ********/
    public void setShuffle(boolean b) {
        shuffle = b;
    }
    
    /********
     * Definit si la playlist recommencera automatiquement ou non
     * 
     * @param b Valeur du repeat
     ********/
    public void setRepeat(boolean b) {
        repeat = b;
    }
    
    /********
     * Affiche le dialogue de selection de fichier
     * Ajoute le fichier a la playlist
     ********/
    public void ajouterChanson() {
        JFileChooser fc = new JFileChooser(player.getLastDir());
        String [] exts = new String [] {"mp3"};
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Fichiers mp3", exts);
        fc.setFileFilter(filter);
        fc.setMultiSelectionEnabled(true);
        
        int file_check = fc.showOpenDialog(this);
        if (file_check == JFileChooser.APPROVE_OPTION) {
            File[] f = fc.getSelectedFiles();
            player.setLastDir(f[0].getParent());
            
            for (int i=0; i<f.length; i++) {
                ajouterChanson(new Chanson(f[i]));
            }
        }
    }
    
    /********
     * Ajoute un fichier a la playlist
     * 
     * @param f Le fichier a ajouter
     ********/
    public void ajouterChanson(Chanson c) {
        //Verification si le fichier existe
        if (c.getFile().exists()) {
            String titre;
            String no = String.valueOf(listeChansons.getRowCount()+1);
            String duree = c.getDuree();
            
            //Aller chercher le titre de la chanson
            if (c.getArtiste().isEmpty() && c.getTitre().isEmpty())
                titre = c.getFile().getName();
            else
                titre = c.getArtiste() + " - " + c.getTitre();
            
            listeChansons.addRow(new String[] {no, titre, duree});
            chansons.add(c);
            
            //Si il n'y a qu'une seule chanson, on la selectionne
            if (chansons.size() == 1) {
                noChansonJouee = 0;
            }
        }
    }
    
    /********
     * Enleve la chanson selectionnee de la playlist
     ********/
    public void enleverChanson() {
        //TODO: Regler bug du titre si j'enleve la chanson qui joue
        
        int no = tableChansons.getSelectedRow();
        
        //Evite un bug si pas de chanson selectionnee
        if (no != -1) {
            listeChansons.removeRow(no);
            chansons.remove(no);
        }
        
        //Renumeroter les chansons
        for (int i=0; i<listeChansons.getRowCount(); i++) {
            listeChansons.setValueAt(i+1, i, 0);
        }
        
        //Selection de la nouvelle chanson du meme no si elle existe
        if (no >= getNbChansons())
            no = getNbChansons()-1;
        if (no >= 0)
            tableChansons.setRowSelectionInterval(no, no);
        
        //Si la chanson qui joue est apres la ligne enlevee, on modifie le noChansonJouee
        if (no < noChansonJouee)
            noChansonJouee--;
    }
    
    /********
     * Change le nom de la chanson
     ********/
    public void renommerChanson() {
        int no = tableChansons.getSelectedRow();
        Chanson c = chansons.get(no);
        
        FrameID3Edit f = new FrameID3Edit(c.getFile());
        f.setVisible(true);
        
        //Mettre le titre a jour
        String nouveauTitre;
        if (c.getArtiste().isEmpty() && c.getTitre().isEmpty())
            nouveauTitre = c.getFile().getName();
        else
            nouveauTitre = c.getArtiste() + " - " + c.getTitre();
        listeChansons.setValueAt(nouveauTitre, no, 1);
    }
    
    /********
     * Deplace la chanson vers le haut
     ********/
    public void deplacerChansonHaut() {
        int no = tableChansons.getSelectedRow();
        
        //Seulement si ce n'est pas la premiere chanson
        if (no > 0) {
            //Si c'est la chanson qui joue en ce moment
            if (no == noChansonJouee)
                noChansonJouee--;
            else if (no-1 == noChansonJouee)
                noChansonJouee++;
            
            //Inverser les chansons
            Chanson temp = chansons.get(no);
            chansons.set(no, chansons.get(no-1));
            chansons.set(no-1, temp);
            
            String titreTemp = (String)listeChansons.getValueAt(no, 1);
            String tempsTemp = (String)listeChansons.getValueAt(no, 2);
            listeChansons.setValueAt(listeChansons.getValueAt(no-1, 1), no, 1);
            listeChansons.setValueAt(listeChansons.getValueAt(no-1, 2), no, 2);
            listeChansons.setValueAt(titreTemp, no-1, 1);
            listeChansons.setValueAt(tempsTemp, no-1, 2);
            
            //Pour s'assurer que les entrees en gras sont les bonnes
            tableChansons.repaint();
        }
    }
    
    /********
     * Deplace la chanson vers le bas
     ********/
    public void deplacerChansonBas() {
        int no = tableChansons.getSelectedRow();
        
        //Seulement si ce n'est pas la premiere chanson
        if (no < getNbChansons()-1) {
            //Si c'est la chanson qui joue en ce moment
            if (no == noChansonJouee)
                noChansonJouee++;
            else if (no+1 == noChansonJouee)
                noChansonJouee--;
            
            //Inverser les chansons
            Chanson temp = chansons.get(no);
            chansons.set(no, chansons.get(no+1));
            chansons.set(no+1, temp);
            
            String titreTemp = (String)listeChansons.getValueAt(no, 1);
            String tempsTemp = (String)listeChansons.getValueAt(no, 2);
            listeChansons.setValueAt(listeChansons.getValueAt(no+1, 1), no, 1);
            listeChansons.setValueAt(listeChansons.getValueAt(no+1, 2), no, 2);
            listeChansons.setValueAt(titreTemp, no+1, 1);
            listeChansons.setValueAt(tempsTemp, no+1, 2);
            
            //Pour s'assurer que les entrees en gras sont les bonnes
            tableChansons.repaint();
        }
    }
    
    /********
     * Cree une nouvelle playlist
     ********/
    public void ajouterPlaylist() {
        player.ajouterPlaylist();
    }
    
    /********
     * Supprime cette playlist
     ********/
    public void enleverPlaylist() {
        player.enleverPlaylist(nomPlaylist);
    }
    
    /********
     * Renomme cette playlist
     ********/
    public void renommerPlaylist() {
        String ancienNom = nomPlaylist;
        String nouveauNom = JOptionPane.showInputDialog(this, "Veuillez entrer le nouveau nom de la playlist", 
                "Renommer la playlist", JOptionPane.QUESTION_MESSAGE);
        
        if (nouveauNom != null && !nouveauNom.isEmpty())
            JOptionPane.showMessageDialog(this, "La playlist ne peut avoir un nom vide", "Attention", JOptionPane.WARNING_MESSAGE);
        else if (nouveauNom != null)
            firePropertyChange("nomPlaylist", ancienNom, nouveauNom);
    }
    
    /********
     * Importe une playlist de format m3u et
     * ajoute les chansons a la playlist courante
     ********/
    public void importerPlaylist() {
        File f = null;
        JFileChooser fc = new JFileChooser(player.getLastDir());
        String [] exts = new String [] {"m3u"};
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Fichiers playlist", exts);
        fc.setFileFilter(filter);
        
        int file_check = fc.showOpenDialog(this);
        if (file_check == JFileChooser.APPROVE_OPTION) {
            try {
                f = fc.getSelectedFile();
                
                player.setLastDir(f.getParent());
                
                FileReader fr = new FileReader(f);
                BufferedReader b = new BufferedReader(fr);
                
                String s = b.readLine();
                while (s != null) {
                    if (s.charAt(0) == '#')
                        continue;
                    System.out.println(s);
                    File f2 = new File(s);
                    if (f2.exists())
                        ajouterChanson(new Chanson(f2));
                    else
                        System.out.println("Le fichier \"" + f2.getName() + "\" n'existe pas.");
                    s = b.readLine();
                }
                
                b.close();
                fr.close();
            } catch (FileNotFoundException fnfe) {
                fnfe.printStackTrace();
            } catch (Exception ioe) {
                ioe.printStackTrace();
                System.out.println("Impossible de lire le fichier \"" + f.getName() + "\".");
            }
        }
    }
    
    /********
     * Exporte une playlist au format m3u
     ********/
    public void exporterPlaylist() {
        File f = null;
        JFileChooser fc = new JFileChooser(player.getLastDir());
        String [] exts = new String [] {"m3u"};
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Fichiers playlist", exts);
        fc.setFileFilter(filter);
        
        int file_check = fc.showSaveDialog(this);
        if (file_check == JFileChooser.APPROVE_OPTION) {
            try {
                f = fc.getSelectedFile();
                
                //Ajouter l'extension .m3u
                if (!f.getName().endsWith(".m3u"))
                    f = new File(f.getParent(), f.getName() + ".m3u");
                
                player.setLastDir(f.getParent());
                
                FileWriter fw = new FileWriter(f, false);
                BufferedWriter b = new BufferedWriter(fw);
                
                for (int i=0; i<chansons.size(); i++) {
                    b.write(chansons.get(i).getFile().getAbsolutePath());
                    b.newLine();
                }
                
                b.flush();
                b.close();
                fw.close();
            } catch (IOException ioe) {
                ioe.printStackTrace();
                System.out.println("Impossible d'ecrire dans le fichier \"" + f.getName() + "\".");
            }
        }
    }
    
    /********
     * Recule d'une chanson dans la liste
     * Ne fait rien si il ne peut reculer (si repeat est false)
     * Ne fait rien si shuffle est active
     * 
     * @return Vrai si la chanson a change
     ********/
    public boolean chansonPrecedente() {
        //Si aucune chanson n'etait choisie pour jouer
        if (noChansonJouee == -1)
            noChansonJouee = 0;
        
        if (shuffle) {
            return false;
        } else if (repeat) {
            noChansonJouee--;
            if (noChansonJouee == -1) {
                noChansonJouee = chansons.size() - 1;
            }
            return true;
        } else if (noChansonJouee > 0) {
            noChansonJouee--;
            return true;
        } else {
            return false;
        }
    }
    
    /********
     * Avance d'une chanson dans la liste
     * Ne fait rien si il ne peut avancer (si repeat est false)
     * Choisit une chanson au hasard si shuffle est active
     * 
     * @return Vrai si la chanson a change
     ********/
    public boolean chansonSuivante() {
        //Si aucune chanson n'etait choisie pour jouer
        if (noChansonJouee == -1)
            noChansonJouee = 0;
        
        if (shuffle) {
            noChansonJouee = (int)(Math.random() * chansons.size());
            return true;
        } else if (repeat) {
            noChansonJouee++;
            if (noChansonJouee == chansons.size()) {
                noChansonJouee = 0;
            }
            return true;
        } else if (noChansonJouee < chansons.size()-1) {
            noChansonJouee++;
            return true;
        } else {
            return false;
        }
    }
    
    /********
     * Active ou desactive le combo box
     * 
     * @param b Active ou non
     ********/
    public void setCmbListesEnabled(boolean b) {
        cmbListes.setEnabled(b);
    }
    
    /********
     * Supprime toutes les chansons d'une playlist
     ********/
    public void viderPlaylist() {
        while (listeChansons.getRowCount() != 0) {
            listeChansons.removeRow(0);
        }
        
        chansons = new Vector<Chanson>();
    }
    
    private class Ecouteur implements ActionListener, KeyListener, MouseListener, MouseMotionListener {
        public void actionPerformed(ActionEvent e) {
            if (e.getSource() == btnAddChanson || e.getSource() == itmAddChanson) {
                ajouterChanson();
            } else if (e.getSource() == btnRemChanson || e.getSource() == itmRemChanson) {
                enleverChanson();
            } else if (e.getSource() == itmRenChanson) {
                renommerChanson();
            } else if (e.getSource() == itmMoveUp) {
                deplacerChansonHaut();
            } else if (e.getSource() == itmMoveDown) {
                deplacerChansonBas();
            } else if (e.getSource() == btnAddPlaylist || e.getSource() == itmAddPLaylist) {
                ajouterPlaylist();
            } else if (e.getSource() == btnRemPlaylist || e.getSource() == itmRemPLaylist) {
                enleverPlaylist();
            } else if (/*e.getSource() == btnRenPlaylist || */e.getSource() == itmRenPlaylist) {
                renommerPlaylist();
            } else if (/*e.getSource() == btnImpPlaylist || */e.getSource() == itmImpPlaylist) {
                importerPlaylist();
            } else if (/*e.getSource() == btnExpPlaylist || */e.getSource() == itmExpPlaylist) {
                exporterPlaylist();
            } else if (e.getSource() == cmbListes) {
                player.changerPlaylist((String)cmbListes.getSelectedItem());
            } else if (e.getSource() == itmPlay) {
                noChansonJouee = tableChansons.getSelectedRow();
                player.demarrerChanson();
            }
        }

        public void keyPressed(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                //Si la cellule est en train d'etre editee, on arrete d'editer
                //mais on ne part pas la chanson
                if (tableChansons.getEditingRow() != -1) {
                    cellEditor.stopCellEditing();
                } else {
                    noChansonJouee = tableChansons.getSelectedRow();
                    player.demarrerChanson();
                }
                
                //Faire en sorte que la table ne traite pas la
                //touche Enter.
                //Application pratique = pas de changement de ligne
                e.consume();
            } else if (e.getKeyCode() == KeyEvent.VK_TAB) {
                //Je veux que Tab change de ligne
                int no = tableChansons.getSelectedRow()+1;
                if (no == getNbChansons())
                    no = 0;
                
                tableChansons.setRowSelectionInterval(no, no);
                
                e.consume();
            } else if (e.getKeyCode() == KeyEvent.VK_DELETE) {
                //Enlever la chanson si on est pas en train d'editer
                if (tableChansons.getEditingRow() == -1) {
                    enleverChanson();
                }
            } else if ((e.getKeyCode() == KeyEvent.VK_UP || e.getKeyCode() == KeyEvent.VK_KP_UP) 
                    && e.isControlDown()) {
                deplacerChansonHaut();
            } else if ((e.getKeyCode() == KeyEvent.VK_DOWN || e.getKeyCode() == KeyEvent.VK_KP_DOWN) 
                    && e.isControlDown()) {
                deplacerChansonBas();
            }
        }

        public void keyReleased(KeyEvent e) {
            // TODO Auto-generated method stub
            
        }

        public void keyTyped(KeyEvent e) {
            // TODO Auto-generated method stub
            
        }

        public void mouseClicked(MouseEvent e) {
            if (e.getButton() == MouseEvent.BUTTON3) {
                tableChansons.setRowSelectionInterval(tableChansons.rowAtPoint(e.getPoint()),
                        tableChansons.rowAtPoint(e.getPoint()));
                popMenu.show(e.getComponent(), e.getX(), e.getY());
            }
            
            //Jouer la chanson suite a un double clic
            if (e.getClickCount() == 2 && e.getButton() == MouseEvent.BUTTON1) {
                noChansonJouee = tableChansons.rowAtPoint(e.getPoint());
                player.demarrerChanson();
            }
        }

        public void mouseEntered(MouseEvent e) {
            // TODO Auto-generated method stub
            
        }

        public void mouseExited(MouseEvent e) {
            // TODO Auto-generated method stub
            
        }

        public void mousePressed(MouseEvent e) {
            //Pour le deplacement de chanson a la souris
            noChansonSelectionnee = tableChansons.rowAtPoint(e.getPoint());
        }

        public void mouseReleased(MouseEvent e) {
            // TODO Auto-generated method stub
            
        }

        public void mouseDragged(MouseEvent e) {
            int noRow = tableChansons.rowAtPoint(e.getPoint());
            //Deplacement de chansons avec un drag de souris
            //noRow devient -1 si je sors de la table en bas
            if (noChansonSelectionnee > noRow && noRow != -1) {
                deplacerChansonHaut();
                noChansonSelectionnee--;
            } else if (noChansonSelectionnee < noRow && noRow != -1) {
                deplacerChansonBas();
                noChansonSelectionnee++;
            }
        }

        public void mouseMoved(MouseEvent e) {
            // TODO Auto-generated method stub
            
        }
        
    }
}
