/**********
 * Cette classe sert a colorer la table de Playlist case par case.
 **********/
package org.acidrain.player;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;

import javax.swing.BorderFactory;
import javax.swing.JTable;
import javax.swing.UIManager;
import javax.swing.table.DefaultTableCellRenderer;

@SuppressWarnings("serial")
public class PlaylistCellRenderer extends DefaultTableCellRenderer {
    private Playlist p;

    public PlaylistCellRenderer(Playlist p) {
        super();
        this.p = p;
    }

    public Component getTableCellRendererComponent(JTable table,
            Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        Component c =  super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);

        /* Je vais chercher la couleur par defaut d'une case
         * et j'en cree deux plus foncees a partir de celle la.
         * La couleur s'ajustera donc en fonction du LAF choisi.
         *
         * Pour la couleur de base, je dois aller chercher celle
         * par defaut du LookAndFeel, sinon il prenait celle de la
         * rangee precedente et la table etait de plus en plus fonce.
         */
        Color cBase = (Color)UIManager.getLookAndFeelDefaults().get("Table.background");
        Color cSelected = cBase.darker();
        Color cFonce = new Color((cBase.getRed() + cSelected.getRed())/2,
                (cBase.getGreen() + cSelected.getGreen())/2, (cBase.getBlue() + cSelected.getBlue())/2);

        //Creer une alternance de couleurs pour les cases, iTunes-like
        if (row%2 == 0 && !isSelected) {
            c.setBackground(cBase);
        } else if (!isSelected) {
            c.setBackground(cFonce);
        } else {
            c.setBackground(cSelected);
            //Enlever la bordure d'une case selectionnee
            setBorder(BorderFactory.createLineBorder(cSelected, 1));
        }

        //Font de base
        Font f = c.getFont();

        //Font pour la chanson en cours
        if (row == p.getNoChansonJouee()) {
            c.setFont(new Font(f.getName(), Font.BOLD, f.getSize()));
        }

        return c;
    }
}
