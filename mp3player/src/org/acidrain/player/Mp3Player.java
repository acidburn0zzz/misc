package org.acidrain.player;

import java.awt.Font;
import java.awt.SystemTray;
import java.awt.TrayIcon;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBoxMenuItem;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.UIManager;
import javax.swing.UIManager.LookAndFeelInfo;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.filechooser.FileNameExtensionFilter;

@SuppressWarnings("serial")
public class Mp3Player extends JFrame {
    private Chanson chanson;
    private WavInfo infoChanson;
    private WavDiffuseur diffuseurChanson;
    private Timer timerSecondes;
    private Timer timerTitre;

    private Hashtable<String, Playlist> listes;
    private Playlist playlist;
    private String nomPlaylistActuelle;

    private ImageIcon icon = new ImageIcon("logo.jpg");

    private JButton btnPrec = new JButton("|<");
    private JButton btnPlay = new JButton(">");
    private JButton btnPause = new JButton("||");
    private JButton btnStop = new JButton("#");
    private JButton btnSuiv = new JButton(">|");
    private JButton btnOuvrir = new JButton("^");
    private JToggleButton btnPlaylist = new JToggleButton("PL");
    private JToggleButton btnShuffle = new JToggleButton("SH");
    private JToggleButton btnRepeat = new JToggleButton("RE");

    private JTextField txtTitre = new JTextField();
    private JTextField txtTemps = new JTextField();
    private JSlider sldTemps = new JSlider(0, 100, 0);
    private JSlider sldVolume = new JSlider(0, 100, 100);

    private JPopupMenu popMenu = new JPopupMenu();
    private JMenuItem itmOuvrir = new JMenuItem("Ouvrir");
    private JMenuItem itmPrec = new JMenuItem("Precedent");
    private JMenuItem itmPlay = new JMenuItem("Play");
    private JMenuItem itmPause = new JMenuItem("Pause");
    private JMenuItem itmStop = new JMenuItem("Stop");
    private JMenuItem itmSuiv = new JMenuItem("Suivant");
    private JCheckBoxMenuItem itmPlaylist = new JCheckBoxMenuItem("Playlist");
    private JCheckBoxMenuItem itmShuffle = new JCheckBoxMenuItem("Shuffle");
    private JCheckBoxMenuItem itmRepeat = new JCheckBoxMenuItem("Repeat");
    private JMenu mnuLAF = new JMenu("Look & Feels");
    private JMenuItem [] itmLAF;
    private JMenuItem itmQuitter = new JMenuItem("Quitter");

    private TrayIcon trayIcon;
    private SystemTray sysTray;

    private String titreChanson;
    private String lastDir;
    private int positionTexte; //Pour animer le titre
    private static final int NB_CHAR_TITRE=40; //Nombre de caracteres a afficher pour le titre
    private static final String NOM_LOGICIEL="Acidrain Mp3 Player";

    private LookAndFeelInfo [] lafs;
    private int noLookAndFeel;

    //Je me base sur la font par defaut d'un bouton pour en faire une plus petite
    private Font fontBoutons = new Font(btnOuvrir.getFont().getFontName(), Font.PLAIN, btnOuvrir.getFont().getSize()-4);
    //Meme chose pour grossir celle de txtTemps
    private Font fontTxtTemps = new Font(txtTemps.getFont().getFontName(), Font.PLAIN, txtTemps.getFont().getSize()+16);

    private boolean mute = false;
    private boolean pause = false;
    private boolean toggleAffichageTemps = false;
    private boolean animerTitre;
    private boolean afficherTempsRestant;
    private boolean showPlaylist;
    private boolean shuffle;
    private boolean repeat;

    private Ecouteur e;

    public Mp3Player() {
        super(NOM_LOGICIEL);
        try {
            jbInit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void jbInit() throws Exception {
        this.setSize(400, 180);
        this.setResizable(false);
        this.setLayout(null);
        this.setFocusable(true);
        this.setIconImage(icon.getImage());

        e = new Ecouteur();
        this.addMouseListener(e);
        this.addKeyListener(e);

        timerSecondes = new Timer(1000, e);
        timerTitre = new Timer(500, e);

        lafs = UIManager.getInstalledLookAndFeels();

        //Disposition
        txtTemps.setBounds(10, 10, 125, 60);
        txtTemps.setEnabled(false);
        txtTemps.setFont(fontTxtTemps);
        txtTemps.setHorizontalAlignment(JTextField.CENTER);
        txtTemps.addMouseListener(e);
        txtTitre.setBounds(145, 10, 238, 25);
        txtTitre.setEnabled(false);
        txtTitre.addMouseListener(e);
        sldVolume.setBounds(145, 45, 152, 25);
        sldVolume.addChangeListener(e);
        sldVolume.addMouseListener(e);
        btnPlaylist.setBounds(307, 45, 76, 25);
        btnPlaylist.addActionListener(e);
        sldTemps.setBounds(10, 80, 383, 25);
        sldTemps.setEnabled(false);
        btnPrec.setBounds(10, 115, 47, 30);
        btnPrec.setFont(fontBoutons);
        btnPrec.addActionListener(e);
        btnPlay.setBounds(57, 115, 47, 30);
        btnPlay.setFont(fontBoutons);
        btnPlay.addActionListener(e);
        btnPause.setBounds(104, 115, 47, 30);
        btnPause.setFont(fontBoutons);
        btnPause.addActionListener(e);
        btnStop.setBounds(151, 115, 47, 30);
        btnStop.setFont(fontBoutons);
        btnStop.addActionListener(e);
        btnSuiv.setBounds(198, 115, 47, 30);
        btnSuiv.setFont(fontBoutons);
        btnSuiv.addActionListener(e);
        btnOuvrir.setBounds(245, 115, 46, 30);
        btnOuvrir.setFont(fontBoutons);
        btnOuvrir.addActionListener(e);
        btnShuffle.setBounds(291, 115, 46, 30);
        btnShuffle.setFont(fontBoutons);
        btnShuffle.addActionListener(e);
        btnRepeat.setBounds(337, 115, 46, 30);
        btnRepeat.setFont(fontBoutons);
        btnRepeat.addActionListener(e);

        this.add(txtTemps);
        this.add(txtTitre);
        this.add(sldVolume);
        this.add(btnPlaylist);
        this.add(sldTemps);
        this.add(btnPrec);
        this.add(btnPlay);
        this.add(btnPause);
        this.add(btnStop);
        this.add(btnSuiv);
        this.add(btnOuvrir);
        this.add(btnShuffle);
        this.add(btnRepeat);

        //Popup menu
        popMenu.add(itmOuvrir);
        itmOuvrir.addActionListener(e);
        popMenu.add(itmPrec);
        itmPrec.addActionListener(e);
        popMenu.add(itmPlay);
        itmPlay.addActionListener(e);
        popMenu.add(itmPause);
        itmPause.addActionListener(e);
        popMenu.add(itmStop);
        itmStop.addActionListener(e);
        popMenu.add(itmSuiv);
        itmSuiv.addActionListener(e);
        popMenu.add(itmPlaylist);
        itmPlaylist.addActionListener(e);
        popMenu.add(itmShuffle);
        itmShuffle.addActionListener(e);
        popMenu.add(itmRepeat);
        itmRepeat.addActionListener(e);
        popMenu.addSeparator();
        itmLAF = new JMenuItem[lafs.length];
        for (int i=0; i<lafs.length; i++) {
            itmLAF[i] = new JMenuItem(lafs[i].getName());
            itmLAF[i].addActionListener(e);
            mnuLAF.add(itmLAF[i]);
        }
        popMenu.add(mnuLAF);
        popMenu.addSeparator();
        popMenu.add(itmQuitter);
        itmQuitter.addActionListener(e);

        //Raccourcis clavier a la Winamp, je n'arrivais pas a
        //les faire fonctionner avec KeyListener
        itmPrec.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Z, KeyEvent.CTRL_MASK));
        itmPlay.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_X, KeyEvent.CTRL_MASK));
        itmPause.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_C, KeyEvent.CTRL_MASK));
        itmStop.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_V, KeyEvent.CTRL_MASK));
        itmSuiv.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_B, KeyEvent.CTRL_MASK));

        itmOuvrir.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_O, KeyEvent.CTRL_MASK));
        itmPlaylist.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_P, KeyEvent.CTRL_MASK));
        itmShuffle.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.CTRL_MASK));
        itmRepeat.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_R, KeyEvent.CTRL_MASK));
        itmQuitter.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q, KeyEvent.CTRL_MASK));

        //TrayIcon
        if (SystemTray.isSupported()) {
            trayIcon = new TrayIcon(icon.getImage(), "AcidRain Mp3 Player");
            trayIcon.setImageAutoSize(true);

            sysTray = SystemTray.getSystemTray();
            sysTray.add(trayIcon);
            trayIcon.addActionListener(e);
        }

        //Chargement de la configuration precedente
        loadConfig();
    }

    /********
     * Chargement de la configuration precedente
     ********/
    public void loadConfig() {
        Configuration c = null;

        try {
            File f = new File(Configuration.getConfigurationPath());
            FileInputStream fis = new FileInputStream(f);
            ObjectInputStream input = new ObjectInputStream(fis);

            c = (Configuration)input.readObject();

            input.close();
            fis.close();
        } catch (FileNotFoundException fnfe) {
            System.out.println("Le fichier \"" + Configuration.getConfigurationPath() + "\" n'existe pas");
            System.out.println("Chargement de la configuration par defaut");
        } catch (IOException ioe) {
            ioe.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (c != null) {
            //On va chercher les paramertes
            Hashtable<String, String> params = c.getParams();
            afficherTempsRestant = Boolean.parseBoolean(params.get("afficherTempsRestant"));
            animerTitre = Boolean.parseBoolean(params.get("animerTitre"));
            showPlaylist = Boolean.parseBoolean(params.get("showPlaylist"));
            shuffle = Boolean.parseBoolean(params.get("shuffle"));
            repeat = Boolean.parseBoolean(params.get("repeat"));
            lastDir = params.get("lastDir");
            nomPlaylistActuelle = params.get("nomPlaylistActuelle");
            noLookAndFeel = Integer.valueOf(params.get("laf"));

            /**********
             * Ceci fait que le slider change de valeur et affiche la
             * valeur du volume dans le textfield.
             */
            sldVolume.setValue(Integer.parseInt(params.get("volume")));
            txtTitre.setText("");

            //On recree les playlists
            listes = new Hashtable<String, Playlist>();

            Vector<File>[] listesFichiers = c.getListesFichiers();
            String[] noms = c.getNoms();
            int[] nosChansonsJouees = c.getNosChansonsJouees();

            //Verification si il y a au moins une playlist
            if (listesFichiers.length > 0) {
                for (int i=0; i<listesFichiers.length; i++) {
                    Playlist temp = new Playlist(this, noms[i], shuffle, repeat);
                    for (int j=0; j<listesFichiers[i].size(); j++) {
                        temp.ajouterChanson(new Chanson(listesFichiers[i].get(j)));
                    }
                    temp.setNoChansonJouee(nosChansonsJouees[i]);
                    temp.addPropertyChangeListener(e);
                    listes.put(noms[i], temp);
                }

                changerPlaylist(nomPlaylistActuelle);
            } else {
                if (!ajouterPlaylist())
                    System.exit(0);
            }

            setPlaylist(showPlaylist);
            setShuffle(shuffle);
            setRepeat(repeat);
        } else { //Valeurs par defaut
            listes = new Hashtable<String, Playlist>();

            //Creation d'une playlist
            if (listes.size() == 0) {
                if (!ajouterPlaylist())
                    //L'utilisateur a clique sur cancel alors qu'il n'y a pas de playlist
                    //On quitte
                    System.exit(0);
            }
            afficherTempsRestant = true;
            animerTitre = true;
            showPlaylist = true;
            shuffle = false;
            repeat = true;
            setPlaylist(showPlaylist);
            setShuffle(shuffle);
            setRepeat(repeat);

            lastDir = "";
            noLookAndFeel = 0;
        }
    }

    /********
     * Cree un objet configuration qui contient ce qui est a sauvegarder
     * L'ecrit dans un fichier
     ********/
    @SuppressWarnings("unchecked")
    public void saveConfig() {
        try {
            Configuration c = new Configuration();

            Hashtable<String, String> params = new Hashtable<String, String>();
            params.put("afficherTempsRestant", String.valueOf(afficherTempsRestant));
            params.put("animerTitre", String.valueOf(animerTitre));
            params.put("showPlaylist", String.valueOf(showPlaylist));
            params.put("shuffle", String.valueOf(shuffle));
            params.put("repeat", String.valueOf(repeat));
            params.put("lastDir", lastDir);
            params.put("nomPlaylistActuelle", nomPlaylistActuelle);
            params.put("volume", String.valueOf(sldVolume.getValue()));
            params.put("laf", String.valueOf(noLookAndFeel));

            int nbPlaylists = listes.size();
            int i=0;
            String noms[] = new String[nbPlaylists];
            Vector<File>[] listesFichiers = new Vector[nbPlaylists];
            int[] nosChansonsJouees = new int[nbPlaylists];

            for (Enumeration<String> en = listes.keys(); en.hasMoreElements();) {
                String s = en.nextElement();
                noms[i] = listes.get(s).getNomPlaylist();
                listesFichiers[i] = listes.get(s).getChansons();
                nosChansonsJouees[i] = listes.get(s).getNoChansonJouee();
                i++;
            }

            c.setParams(params);
            c.setListesFichiers(listesFichiers);
            c.setNoms(noms);
            c.setNosChansonsJouees(nosChansonsJouees);

            //Ecriture dans le fichier
            File f = new File(Configuration.getConfigurationPath());
            FileOutputStream fos = new FileOutputStream(f, false);
            ObjectOutputStream output = new ObjectOutputStream(fos);

            output.writeObject(c);
            output.flush();
            output.close();
            fos.close();
        } catch (FileNotFoundException fnfe) {
            fnfe.printStackTrace();
        } catch (IOException ioe) {
            ioe.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /********
     * Retourne le dernier repertoire ouvert
     *
     * @return Le repertoire
     ********/
    public String getLastDir() {
        return lastDir;
    }

    /********
     * Change la valeur du dernier repertoire ouvert
     *
     * @param s La nouvelle valeur
     ********/
    public void setLastDir(String s) {
        lastDir = s;
    }

    /********
     * Retourne la hashtable qui contient les playlists
     *
     * @return Les playlists
     ********/
    public Hashtable<String, Playlist> getListes() {
        return listes;
    }

    /********
     * Change la playlist en cours
     *
     * @param s Le nom de la playlist
     ********/
    public void setListe(String nom) {
        playlist = listes.get(nom);
    }

    /********
     * Retourne le statut de la chanson qui joue
     *
     * @return Statut de la chanson ou STOP si pas de chanson
     ********/
    public int getStatut() {
        if (diffuseurChanson != null)
            return diffuseurChanson.getStatut();
        else
            return WavDiffuseur.STOP;
    }

    /********
     * Affiche le dialogue de selection de fichier
     * Demarre la chanson
     ********/
    public void ouvrir() {
        JFileChooser fc = new JFileChooser(lastDir);
        String [] exts = new String [] {"mp3"};
        FileNameExtensionFilter filter = new FileNameExtensionFilter("Fichiers mp3", exts);
        fc.setFileFilter(filter);
        fc.setMultiSelectionEnabled(true);

        int file_check = fc.showOpenDialog(this);
        if (file_check == JFileChooser.APPROVE_OPTION) {
            File[] f = fc.getSelectedFiles();
            lastDir = f[0].getParent();

            playlist.viderPlaylist();

            for (int i=0; i<f.length; i++) {
                playlist.ajouterChanson(new Chanson(f[i]));
            }

            demarrerChanson();
        }
    }

    /********
     * Demarre la chanson a partir du debut
     ********/
    public void demarrerChanson() {
        //On ferme la chanson en cours
        stopperChanson();

        //On va chercher la chanson en cours dans la playlist
        chanson = playlist.getChanson();

        //fichierChanson sera null si il n'y a rien dans la playlist
        if (chanson != null) {
            //On ouvre la chanson
            infoChanson = new WavInfo(chanson.getFile());

            //On demarre la chanson si il n'y a pas eu d'erreur a l'ouverture
            if (infoChanson.getNbSec() > 0) {
                positionTexte = 0;

                sldTemps.setMaximum(infoChanson.getNbSec());
                titreChanson = chanson.getTitre()+ " (" + chanson.getDuree() + ")";

                diffuseurChanson = new WavDiffuseur(infoChanson, sldVolume.getValue());
                diffuseurChanson.start();

                //Animation titre et desactiver le combobox
                animationTitre();
                playlist.setCmbListesEnabled(false);

                timerSecondes.start();
                timerTitre.start();
            }
        } else {
            //Si la playlist est vide, on va ouvrir une chanson
            ouvrir();
        }
    }

    /********
     * Joue la chanson du debut si elle est stoppee ou en cours de lecture
     * Joue la chanson de sa position actuelle si elle est sur pause
     ********/
    public void jouerChanson() {
        if (diffuseurChanson != null && diffuseurChanson.getStatut() == WavDiffuseur.PAUSE) {
            diffuseurChanson.setStatut(WavDiffuseur.PLAY);
            pause = false;
        } else {
            demarrerChanson();
        }
    }

    /********
     * Toggle entre play et pause
     ********/
    public void pauseChanson() {
        if (diffuseurChanson != null) {
            if (diffuseurChanson.getStatut() == WavDiffuseur.PLAY) {
                diffuseurChanson.setStatut(WavDiffuseur.PAUSE);
                pause = true;
                toggleAffichageTemps = false;
            } else {
                diffuseurChanson.setStatut(WavDiffuseur.PLAY);
                pause = false;
            }
        }
    }

    /********
     * Arrete la chanson
     ********/
    public void stopperChanson() {
        if (diffuseurChanson != null) {
            diffuseurChanson.setStatut(WavDiffuseur.STOP);
            sldTemps.setValue(0);
            txtTemps.setText("");
            timerSecondes.stop();
            playlist.setCmbListesEnabled(true);
        }
    }

    /********
     * Anime le titre de la chanson si la chanson joue
     * si le titre est plus long que "NB_CHAR_TITRE" caracteres
     ********/
    public void animationTitre() {
        if (diffuseurChanson != null) {
            String nouveauTitre;

            if (chanson.getArtiste().isEmpty() && chanson.getTitre().isEmpty())
                //Titre par defaut si le tag est vide
                nouveauTitre = chanson.getFile().getName();
            else
                nouveauTitre = chanson.getArtiste() + " - " + chanson.getTitre() +
                    " (" + chanson.getDuree() + ")";

            //On met a jour le titre de la chanson si il a ete modifie
            //et on remet le compteur position a 0
            if (!titreChanson.equals(nouveauTitre)) {
                titreChanson = nouveauTitre;
                positionTexte = 0;
            }

            String titre;

            if (animerTitre) {
                int positionFin;

                if (positionTexte == titreChanson.length())
                    positionTexte = 0;

                if (positionTexte+NB_CHAR_TITRE > titreChanson.length())
                    positionFin = titreChanson.length();
                else
                    positionFin = positionTexte+NB_CHAR_TITRE;

                titre = titreChanson.substring(positionTexte, positionFin);
            } else {
                if (NB_CHAR_TITRE <= titreChanson.length()) {
                    titre = titreChanson.substring(0, NB_CHAR_TITRE);
                } else {
                    titre = titreChanson;
                }
            }

            txtTitre.setText(titre);
            this.setTitle(playlist.getNoChansonJouee()+1 + ". " + titre + " - " + NOM_LOGICIEL);

            //Titre dans le tooltip du TrayIcon
            //N'est pas anime
            trayIcon.setToolTip(titreChanson);
        } else {
            txtTitre.setText("");
            this.setTitle(NOM_LOGICIEL);
            trayIcon.setToolTip(NOM_LOGICIEL);
        }
    }

    /********
     * Affiche le temps ecoule ou restant a la chanson
     ********/
    public void updateTemps() {
        if (diffuseurChanson != null) {
            String sTemps = "";
            int iTemps;

            if (afficherTempsRestant) {
                iTemps = diffuseurChanson.getNbSecRestantes();
                sTemps += "-";
            } else {
                iTemps = diffuseurChanson.getNbSecJouees();
            }

            sTemps += nbSecToString(iTemps);

            //Pour faire "flasher" le temps si en pause
            if (pause) {
                toggleAffichageTemps = !toggleAffichageTemps;
                if (toggleAffichageTemps)
                    sTemps = "";
            }

            txtTemps.setText(sTemps);
            sldTemps.setValue(diffuseurChanson.getNbSecJouees());
        }
    }

    /********
     * Pour changer le volume
     * Affiche le % a la place du titre
     ********/
    public void changerVolume() {
        if (diffuseurChanson != null) {
            diffuseurChanson.setVolume(sldVolume.getValue());
        }
        txtTitre.setText("Volume : " + sldVolume.getValue() + "%");
    }

    /********
     * Cree une nouvelle playlist et l'ajoute au Hashtable
     *
     * @return Si la creation a fonctionne
     ********/
    public boolean ajouterPlaylist() {
        String nom = JOptionPane.showInputDialog(this, "Veuillez entrer un nom pour la nouvelle playlist"
                , "Nouvelle playlist", JOptionPane.QUESTION_MESSAGE);

        //L'utilisateur a clique sur cancel
        if (nom == null)
            return false;

        //Verification si il y a un nom
        if (nom.isEmpty()) {
            JOptionPane.showMessageDialog(this, "Une playlist ne peut pas avoir un nom vide", "Erreur", JOptionPane.ERROR_MESSAGE);
            return ajouterPlaylist();
        }

        //Verification si le nom est unique
        if (listes.containsKey(nom)) {
            JOptionPane.showMessageDialog(this, "Une playlist du nom de \"" + nom + "\" existe deja.", "Erreur", JOptionPane.ERROR_MESSAGE);
            return ajouterPlaylist();
        }

        Playlist temp = new Playlist(this, nom, shuffle, repeat);
        temp.addPropertyChangeListener(e);
        listes.put(nom, temp);
        changerPlaylist(nom);

        return true;
    }

    /********
     * Change la playlist affichee
     *
     * @param nom Le nom de la playlist a afficher
     ********/
    public void changerPlaylist(String nom) {
        try {
            this.remove(playlist);
        } catch (NullPointerException npe) {
            //Arrive quand il n'y a pas de playlist
            //Erreur ignoree
        } catch (Exception e) {
            e.printStackTrace();
        }

        playlist = listes.get(nom);
        nomPlaylistActuelle = nom;
        playlist.setBounds(10, 170, 373, 300);
        this.add(playlist);
        playlist.createComboBox();
        playlist.setShuffle(shuffle);
        playlist.setRepeat(repeat);
        setLookAndFeel(noLookAndFeel);
    }

    /********
     * Supprimer une playlisr
     *
     * @param nom Le nom de la playlist a supprimer
     ********/
    public void enleverPlaylist(String nom) {
        stopperChanson();
        this.remove(playlist);
        listes.remove(nom);

        //Si il reste une playlist, on choisit la premiere
        //Sinon on en cree une autre
        if (listes.size() > 0) {
            changerPlaylist(listes.keys().nextElement());
        } else {
            this.repaint();
            if (!ajouterPlaylist())
                this.dispose();
        }
    }

    /********
     * Renommer une playlist
     *
     * @param ancienNom Le nom de la playlist a renommer
     * @param nouveauNom Le nouveau nom de la playlist
     * @return True si le changement a eu lieu
     ********/
    public boolean renommerPlaylist(String ancienNom, String nouveauNom) {
        //Verification si une playlist nouveauNom existe
        if (listes.containsKey(nouveauNom)) {
            JOptionPane.showMessageDialog(this, "Il existe deja une playlist du nom de " + nouveauNom,
                    "Attention", JOptionPane.WARNING_MESSAGE);
            return false;
        }

        //Renommer la playlist
        Playlist temp = listes.get(ancienNom);
        listes.remove(ancienNom);
        listes.put(nouveauNom, temp);

        temp = listes.get(nouveauNom);
        temp.setNomPlaylist(nouveauNom);
        temp.createComboBox();
        changerPlaylist(nouveauNom);
        return true;
    }

    /********
     * Recule d'une chanson si possible
     * Joue la nouvelle chanson si on a recule
     ********/
    public void chansonPrecedente() {
        if (playlist.chansonPrecedente()) {
            demarrerChanson();
        }
    }

    /********
     * Recule d'une chanson si possible
     * Joue la nouvelle chanson si on a recule
     ********/
    public void chansonSuivante() {
        if (playlist.chansonSuivante()) {
            demarrerChanson();
        }
    }

    /********
     * Alterne entre mute ou pas
     ********/
    public void toggleMute() {
        if (diffuseurChanson != null) {
            diffuseurChanson.setMute(mute);
            mute = !mute;
        }
    }

    /********
     * Definit si la playlist est visible ou non
     *
     * @param b Visible ou non
     ********/
    public void setPlaylist(boolean b) {
        showPlaylist = b;
        btnPlaylist.setSelected(b);
        itmPlaylist.setSelected(b);

        if (b) {
            this.setSize(400, 500);
            playlist.setVisible(true);
            playlist.repaint();
        } else {
            this.setSize(400, 180);
            playlist.setVisible(false);
        }
    }

    /********
     * Definit si les chansons joueront en ordre ou non
     *
     * @param b Shuffle ou non
     ********/
    public void setShuffle(boolean b) {
        shuffle = b;
        btnShuffle.setSelected(b);
        itmShuffle.setSelected(b);

        playlist.setShuffle(b);
    }

    /********
     * Definit si la playlist se repetera ou nons
     *
     * @param b Repeat ou non
     ********/
    public void setRepeat(boolean b) {
        repeat = b;
        btnRepeat.setSelected(b);
        itmRepeat.setSelected(b);

        playlist.setRepeat(b);
    }

    /********
     * Change le look and feel du logiciel
     *
     * @param no L'index du Look And Feel dans la liste lafs
     ********/
    public void setLookAndFeel(int no) {
        noLookAndFeel = no;
        try {
            UIManager.setLookAndFeel(lafs[no].getClassName());
            //Fait en sorte que la couleur des textfield disabled soit
            //la meme que celle des textfield enabled
            UIManager.put("TextField.inactiveForeground", UIManager.get("TextField.foreground"));//TextField.foreground
            SwingUtilities.updateComponentTreeUI(this);
            SwingUtilities.updateComponentTreeUI(popMenu);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /********
     * Convertit un nombre de secondes en
     * un string de type m:ss
     *
     * @param nbSec Le nombre de secondes a convertir
     * @return Le temps au format m:ss
     ********/
    public static String nbSecToString(int nbSec) {
        String min, sec;

        min = String.valueOf(nbSec/60);
        sec = String.valueOf(nbSec%60);
        if (sec.length() == 1) {
            sec = "0" + sec;
        }

        return min + ":" + sec;
    }

    /********
     * Arreter les timers et la chanson avant de fermer
     ********/
    public void dispose() {
        timerSecondes.stop();
        timerTitre.stop();
        if (diffuseurChanson != null)
            diffuseurChanson.setStatut(WavDiffuseur.STOP);
        saveConfig();
        System.exit(0);
    }

    public static void main(String [] args) {
        Mp3Player pl = new Mp3Player();
        pl.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        pl.setVisible(true);
    }

    private class Ecouteur implements ActionListener, ChangeListener, MouseListener, PropertyChangeListener, KeyListener {
        public void actionPerformed(ActionEvent e) {
            if (e.getSource() == btnPrec || e.getSource() == itmPrec) {
                chansonPrecedente();
            } else if (e.getSource() == btnPlay || e.getSource() == itmPlay) {
                jouerChanson();
            } else if (e.getSource() == btnPause || e.getSource() == itmPause) {
                pauseChanson();
            } else if (e.getSource() == btnStop || e.getSource() == itmStop) {
                stopperChanson();
            } else if (e.getSource() == btnSuiv || e.getSource() == itmSuiv) {
                chansonSuivante();
            } else if (e.getSource() == btnOuvrir || e.getSource() == itmOuvrir) {
                ouvrir();
            } else if (e.getSource() == btnPlaylist || e.getSource() == itmPlaylist) {
                setPlaylist(!showPlaylist);
            } else if (e.getSource() == btnShuffle || e.getSource() == itmShuffle) {
                setShuffle(!shuffle);
            } else if (e.getSource() == btnRepeat || e.getSource() == itmRepeat) {
                setRepeat(!repeat);
            } else if (e.getSource() == timerSecondes) {
                updateTemps();
                if (diffuseurChanson.getStatut() == WavDiffuseur.STOP) {
                    stopperChanson();
                    chansonSuivante();
                    if (!Mp3Player.this.isVisible())
                    	trayIcon.displayMessage(chanson.getTitre(), chanson.getArtiste(), TrayIcon.MessageType.INFO);
                }
            } else if (e.getSource() == timerTitre) {
                positionTexte++;
                animationTitre();
            } else if (e.getSource() == itmQuitter) {
                Mp3Player.this.dispose();
            } else if (e.getSource() == trayIcon) {
	            Mp3Player.this.setVisible(!Mp3Player.this.isVisible());
            } else {
               for (int i=0; i<lafs.length; i++) {
                   if (e.getSource() == itmLAF[i]) {
                       setLookAndFeel(i);
                   }
               }
            }
        }

        public void stateChanged(ChangeEvent arg0) {
            changerVolume();
        }

        public void mouseClicked(MouseEvent arg0) {
            if (arg0.getSource() == txtTitre) {
                animerTitre = !animerTitre;
                positionTexte = 0;
                animationTitre();
            } else if (arg0.getSource() == txtTemps) {
                afficherTempsRestant = !afficherTempsRestant;
                updateTemps();
            } else if (arg0.getButton() == MouseEvent.BUTTON3) {
                popMenu.show(arg0.getComponent(), arg0.getX(), arg0.getY());
            }
        }

        public void mouseEntered(MouseEvent arg0) {
            // TODO Auto-generated method stub

        }

        public void mouseExited(MouseEvent arg0) {
            // TODO Auto-generated method stub

        }

        public void mousePressed(MouseEvent arg0) {
            if (arg0.getSource() == sldVolume) {
                //Arreter le timer pour ne pas que le titre s'affiche
                //a la place tu volume
                timerTitre.stop();
            }
        }

        public void mouseReleased(MouseEvent arg0) {
            if (arg0.getSource() == sldVolume) {
                if (diffuseurChanson != null)
                    timerTitre.start();
            }
        }

        public void propertyChange(PropertyChangeEvent evt) {
            if (evt.getPropertyName().equals("nomPlaylist"))
                renommerPlaylist((String)evt.getOldValue(), (String)evt.getNewValue());
        }

        public void keyPressed(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_Z) {
                chansonPrecedente();
            } else if (e.getKeyCode() == KeyEvent.VK_X) {
                jouerChanson();
            } else if (e.getKeyCode() == KeyEvent.VK_C) {
                pauseChanson();
            } else if (e.getKeyCode() == KeyEvent.VK_V) {
                stopperChanson();
            } else if (e.getKeyCode() == KeyEvent.VK_B) {
                chansonSuivante();
            }
        }

        public void keyReleased(KeyEvent e) {
            // TODO Auto-generated method stub

        }

        public void keyTyped(KeyEvent e) {
            // TODO Auto-generated method stub

        }
    }
}
