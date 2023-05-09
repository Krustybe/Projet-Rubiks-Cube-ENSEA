package com.example.projectrubikscube;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    // Tableau de caractères qui représente le contenu du Rubik's cube
    private char[][] colorSymbols = new char[6][9];

    // Tableau d'entiers qui représente les couleurs de chaque face du cube
    private static final int[] COLORS = {
            R.color.gray, R.color.green, R.color.yellow,
            R.color.blue, R.color.red, R.color.orange
    };

    // Tableau d'entiers qui contient les identifiants des vues des faces du cube
    private static final int[][] IDS = {
            {R.id.cubeImageView_U1, R.id.cubeImageView_U2, R.id.cubeImageView_U3,
                    R.id.cubeImageView_U4, R.id.cubeImageView_U5, R.id.cubeImageView_U6,
                    R.id.cubeImageView_U7, R.id.cubeImageView_U8, R.id.cubeImageView_U9},
            {R.id.cubeImageView_F1, R.id.cubeImageView_F2, R.id.cubeImageView_F3,
                    R.id.cubeImageView_F4, R.id.cubeImageView_F5, R.id.cubeImageView_F6,
                    R.id.cubeImageView_F7, R.id.cubeImageView_F8, R.id.cubeImageView_F9},
            {R.id.cubeImageView_D1, R.id.cubeImageView_D2, R.id.cubeImageView_D3,
                    R.id.cubeImageView_D4, R.id.cubeImageView_D5, R.id.cubeImageView_D6,
                    R.id.cubeImageView_D7, R.id.cubeImageView_D8, R.id.cubeImageView_D9},
            {R.id.cubeImageView_B1, R.id.cubeImageView_B2, R.id.cubeImageView_B3,
                    R.id.cubeImageView_B4, R.id.cubeImageView_B5, R.id.cubeImageView_B6,
                    R.id.cubeImageView_B7, R.id.cubeImageView_B8, R.id.cubeImageView_B9},
            {R.id.cubeImageView_L1, R.id.cubeImageView_L2, R.id.cubeImageView_L3,
                    R.id.cubeImageView_L4, R.id.cubeImageView_L5, R.id.cubeImageView_L6,
                    R.id.cubeImageView_L7, R.id.cubeImageView_L8, R.id.cubeImageView_L9},
            {R.id.cubeImageView_R1, R.id.cubeImageView_R2, R.id.cubeImageView_R3,
                    R.id.cubeImageView_R4, R.id.cubeImageView_R5, R.id.cubeImageView_R6,
                    R.id.cubeImageView_R7, R.id.cubeImageView_R8, R.id.cubeImageView_R9}
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialise le tableau colorSymbols avec des X pour toutes les cases, et les lettres correspondant aux couleurs pour les cases du centre
        for (int i = 0; i < colorSymbols.length; i++) {
            for (int j = 0; j < colorSymbols[i].length; j++) {
                colorSymbols[i][j] = 'X';
            }
        }
        colorSymbols[0][4] = 'W';
        colorSymbols[1][4] = 'G';
        colorSymbols[2][4] = 'Y';
        colorSymbols[3][4] = 'B';
        colorSymbols[4][4] = 'R';
        colorSymbols[5][4] = 'O';

        // Ajout d'un listener aux ImageViews représentant les cases du Rubik's Cube
        // Lorsque l'utilisateur touche l'ImageView, le RubiksCubeTouchListener est appelé
        // avec la matrice de symboles de couleurs en paramètre
        for (int i = 0; i < IDS.length; i++) {
            for (int j = 0; j < IDS[i].length; j++) {
                ImageView cubeImageView = findViewById(IDS[i][j]);
                cubeImageView.setOnTouchListener(new RubiksCubeTouchListener(IDS, colorSymbols));
                if (j == 4) {
                    cubeImageView.setColorFilter(ContextCompat.getColor(this, COLORS[i]));
                }
            }
        }

        // Ajout d'un listener au bouton "Affiche Cube"
        // Lorsque l'utilisateur clique sur le bouton, la méthode displayColorSymbols est appelée
        Button buttonAffiche = findViewById(R.id.btn_affiche_cube);
        buttonAffiche.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                displayColorSymbols();
            }
        });


        // Ajout d'un listener au bouton "Send Cube"
        // Lorsque l'utilisateur clique sur le bouton, la méthode sendWiFiMessage est appelée
        // avec la matrice de symboles de couleurs en paramètre
        Button buttonSend = findViewById(R.id.btn_send_cube);
        buttonSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                sendWiFiMessage(colorSymbols);
            }
        });

        Button buttonExecute = findViewById(R.id.btn_execute);
        buttonExecute.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                executeCubeProgram();
            }
        });

        Button buttonCamera = findViewById(R.id.btn_camera);
        buttonCamera.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                sendWiFiMessageForCamera();
            }
        });
    }

    private void sendWiFiMessageForCamera() {
        new SendWiFiMessageForCameraTask().execute();
    }

    private static class SendWiFiMessageForCameraTask extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params) {
            try {
                // Créez un message pour dire au serveur d'exécuter le programme camera.exe
                String message = "EXECUTE_CAMERA_PROGRAM";

                // Envoi du message par WiFi
                Socket socket = new Socket();
                SocketAddress socketAddress = new InetSocketAddress("192.168.1.193", 12345);
                socket.connect(socketAddress, 10000);
                OutputStream outputStream = socket.getOutputStream();
                outputStream.write(message.getBytes());
                outputStream.close();
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
    }

    private void executeCubeProgram() {
        new ExecuteCubeProgramTask().execute();
    }

    private static class ExecuteCubeProgramTask extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... params) {
            try {
                // Créez un message pour dire au serveur d'exécuter le programme cube.exe
                String message = "EXECUTE_CUBE_PROGRAM";

                // Envoi du message par WiFi
                Socket socket = new Socket();
                SocketAddress socketAddress = new InetSocketAddress("192.168.1.193", 12345);
                socket.connect(socketAddress, 10000);
                OutputStream outputStream = socket.getOutputStream();
                outputStream.write(message.getBytes());
                outputStream.close();
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
    }

    private void displayColorSymbols() {
        StringBuilder sb = new StringBuilder();

        // Parcours la matrice de symboles de couleurs pour construire la chaîne de caractères à afficher
        for (int i = 0; i < colorSymbols.length; i++) {
            for (int j = 0; j < colorSymbols[i].length; j++) {
                sb.append(colorSymbols[i][j]);
                sb.append(" ");
            }
            sb.append("\n");
        }

        // Création d'une AlertDialog pour afficher la chaîne de caractères
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Cube Content");
        builder.setMessage(sb.toString());

        // Ajout d'un bouton "OK" pour fermer la fenêtre
        builder.setPositiveButton("OK", (dialog, which) -> dialog.dismiss());
        builder.create().show();
    }

    private void sendWiFiMessage(char[][] colorSymbols) {
        new SendWiFiMessageTask().execute(colorSymbols);
    }

    private static class SendWiFiMessageTask extends AsyncTask<char[][], Void, Void> {
        @Override
        protected Void doInBackground(char[][]... params) {
            try {
                char[][] colorSymbols = params[0];

                // Construction de la chaîne de caractères contenant le contenu du cube
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < colorSymbols.length; i++) {
                    for (int j = 0; j < colorSymbols[i].length; j++) {
                        sb.append(colorSymbols[i][j]);
                        sb.append("");
                    }
                    sb.append("\n");
                }

                String message = sb.toString();

                // Envoi du message par WiFi
                Socket socket = new Socket();
                SocketAddress socketAddress = new InetSocketAddress("192.168.1.193", 12345);
                socket.connect(socketAddress, 10000);
                OutputStream outputStream = socket.getOutputStream();
                outputStream.write(message.getBytes());
                outputStream.close();
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return null;
        }
    }
}