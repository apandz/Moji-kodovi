package ba.unsa.etf.rpr;

import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.util.ArrayList;

public class DrzavaController {
    public TextField fieldNaziv;
    public ChoiceBox<Grad> choiceGrad;
    private SimpleStringProperty naziv;

    private boolean ok;

    private ObservableList<Grad> gradovi;

    public DrzavaController(Drzava drzava, ArrayList<Grad> gradovi) {
        naziv=new SimpleStringProperty("");
        this.gradovi= FXCollections.observableArrayList(gradovi);
        ok=false;
    }

    @FXML
    public void initialize() {
        fieldNaziv.textProperty().bindBidirectional(naziv);
        choiceGrad.setItems(gradovi);
    }

    public void zatvoriProzor(ActionEvent actionEvent) {
        Node n = (Node) actionEvent.getSource();
        Stage stage = (Stage) n.getScene().getWindow();
        stage.close();
    }

    public void validiraj(ActionEvent actionEvent) {
        if(naziv.getValue().trim().isEmpty()) {
            fieldNaziv.getStyleClass().removeAll("poljeJeIspravno");
            fieldNaziv.getStyleClass().add("poljeNijeIspravno");
        } else {
            fieldNaziv.getStyleClass().removeAll("poljeNijeIspravno");
            fieldNaziv.getStyleClass().add("poljeJeIspravno");
            ok=true;
        }
        if(ok) {
            zatvoriProzor(actionEvent);
        }
    }

    public Drzava getDrzava() {
        if(ok) return new Drzava(0,naziv.getValue(),(Grad)choiceGrad.getValue());
        return null;
    }
}
