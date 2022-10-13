package ba.unsa.etf.rpr.zadaca3.controller;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.Alert;
import javafx.scene.control.ProgressBar;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

public class NoviController {
    public TextField fldIme;
    public ProgressBar progressBar;
    public boolean validnoIme=false;

    public NoviController() {}

    @FXML
    public void initialize() {
        fldIme.textProperty().addListener((observableValue, o, n) -> {
            if(!o.equals(n)) {
                if (n.strip().length() < 10) {
                    progressBar.setProgress(n.strip().length() / 10.);
                    progressBar.getStyleClass().removeAll("zeleniProgress");
                    progressBar.getStyleClass().add("crveniProgress");
                } else {
                    progressBar.setProgress(1.0);
                    progressBar.getStyleClass().removeAll("crveniProgress");
                    progressBar.getStyleClass().add("zeleniProgress");
                }
            }
        });
    }

    public void validiraj(ActionEvent actionEvent) {
        if(fldIme.getText().length()>=10) {
            validnoIme=true;
            zatvori(actionEvent);
        } else {
            Alert alert=new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Neispravno ime");
            alert.setHeaderText("Neispravno ime studenta");
            alert.setContentText("Ime studenta treba biti najmanje 10 karaktera dugačko");
            alert.show();
        }
    }

    public void zatvori(ActionEvent actionEvent) {
        Node n = (Node) actionEvent.getSource();
        Stage stage = (Stage) n.getScene().getWindow();
        stage.close();
    }
}
