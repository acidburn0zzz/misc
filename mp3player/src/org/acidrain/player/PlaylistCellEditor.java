/**********
 * Cette classe sert a rendre la table de
 * Playlist non-editable, sauf si specifie.
 **********/
package org.acidrain.player;

import java.util.EventObject;

import javax.swing.DefaultCellEditor;
import javax.swing.JTextField;

@SuppressWarnings("serial")
public class PlaylistCellEditor extends DefaultCellEditor {
    private boolean editable;

    public PlaylistCellEditor(JTextField textField) {
        super(textField);
        editable = false;
    }

    public void setEditable(boolean editable) {
        this.editable = editable;
    }

    public boolean isCellEditable(EventObject e) {
        return editable;
    }
}
