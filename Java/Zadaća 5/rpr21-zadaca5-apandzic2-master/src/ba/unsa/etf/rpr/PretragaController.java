package ba.unsa.etf.rpr;

import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;

import java.io.File;
import java.util.ArrayList;

public class PretragaController {
    public ListView<String> puteviView;
    public TextField fldUzorak;

    private ObservableList<String> apsolutniPutevi;

    PretragaController() {
    }

    @FXML
    public void initialize() {
        apsolutniPutevi= FXCollections.observableArrayList(new ArrayList<>());
        puteviView.setItems(apsolutniPutevi);
    }

    public void traziAction(ActionEvent actionEvent) {
        apsolutniPutevi.clear();
        if(fldUzorak==null) {
            fldUzorak.setText("");
        }
        Thread thread=new Thread(()->{
            traziPuteve(fldUzorak.getText(),new File(System.getProperty("user.home")));
        });
        thread.start();
    }

    private void traziPuteve(String text, File file) {
        File[] files=file.listFiles();
        if(files!=null) {
            for (File file1 : files) {
                if (file1.getAbsoluteFile().toString().toLowerCase().contains(text.toLowerCase())) {
                    Platform.runLater(() -> apsolutniPutevi.add(file1.getAbsoluteFile().toString()));
                } else {
                    traziPuteve(text, file1);
                }
            }
        }
    }
}
