package com.example.projectrubikscube;

// On importe les classes nécessaires
import android.graphics.Color;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

// On crée une classe qui implémente l'interface OnTouchListener
public class RubiksCubeTouchListener implements View.OnTouchListener {

    // On définit un tableau de couleurs et un tableau de symboles correspondant à chaque face du Rubik's Cube
    private static final int[] COLORS = {
            Color.BLUE, Color.GREEN, Color.RED, Color.rgb(255, 165, 0), Color.GRAY, Color.YELLOW
    };
    private static final char[] SYMBOLS = {'B', 'G', 'R', 'O', 'W', 'Y'};

    // On initialise les variables nécessaires pour la mise à jour des couleurs dans la matrice colorSymbols
    private int colorIndex = 0;
    private int[][] IDS;
    private char[][] colorSymbols;

    // Constructeur de la classe
    public RubiksCubeTouchListener(int[][] IDS, char[][] colorSymbols) {
        this.IDS = IDS;
        this.colorSymbols = colorSymbols;
    }

    // Méthode onTouch qui est appelée lorsqu'on touche l'écran
    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            // On récupère l'ImageView sur laquelle on a appuyé et on change sa couleur
            ImageView imageView = (ImageView) v;
            imageView.setColorFilter(COLORS[colorIndex]);

            // On met à jour la matrice colorSymbols en fonction de la face touchée
            for (int i = 0; i < IDS.length; i++) {
                for (int j = 0; j < IDS[i].length; j++) {
                    if (IDS[i][j] == v.getId()) {
                        colorSymbols[i][j] = SYMBOLS[colorIndex];
                    }
                }
            }

            // On change l'indice de couleur pour le prochain appui
            colorIndex = (colorIndex + 1) % COLORS.length;
            return true;
        }
        return false;
    }
}
