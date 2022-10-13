package ba.unsa.etf.rpr.t7;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Insets;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.layout.GridPane;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import net.sf.jasperreports.engine.JRException;

import java.io.File;
import java.io.IOException;
import java.util.Locale;
import java.util.ResourceBundle;

public class KorisnikController {
    public TextField fldIme;
    public TextField fldPrezime;
    public TextField fldEmail;
    public TextField fldUsername;
    public ListView<Korisnik> listKorisnici;
    public PasswordField fldPassword;
    private boolean eng;

    private KorisniciModel model;

    public KorisnikController(KorisniciModel model) {
        eng=true;
        this.model = model;
    }

    @FXML
    public void initialize() {
        listKorisnici.setItems(model.getKorisnici());
        listKorisnici.getSelectionModel().selectedItemProperty().addListener((obs, oldKorisnik, newKorisnik) -> {
            model.setTrenutniKorisnik(newKorisnik);
            listKorisnici.refresh();
         });

        model.trenutniKorisnikProperty().addListener((obs, oldKorisnik, newKorisnik) -> {
            if (oldKorisnik != null) {
                fldIme.textProperty().unbindBidirectional(oldKorisnik.imeProperty() );
                fldPrezime.textProperty().unbindBidirectional(oldKorisnik.prezimeProperty() );
                fldEmail.textProperty().unbindBidirectional(oldKorisnik.emailProperty() );
                fldUsername.textProperty().unbindBidirectional(oldKorisnik.usernameProperty() );
                fldPassword.textProperty().unbindBidirectional(oldKorisnik.passwordProperty() );
            }
            if (newKorisnik == null) {
                fldIme.setText("");
                fldPrezime.setText("");
                fldEmail.setText("");
                fldUsername.setText("");
                fldPassword.setText("");
            }
            else {
                fldIme.textProperty().bindBidirectional( newKorisnik.imeProperty() );
                fldPrezime.textProperty().bindBidirectional( newKorisnik.prezimeProperty() );
                fldEmail.textProperty().bindBidirectional( newKorisnik.emailProperty() );
                fldUsername.textProperty().bindBidirectional( newKorisnik.usernameProperty() );
                fldPassword.textProperty().bindBidirectional( newKorisnik.passwordProperty() );
            }
        });

        fldIme.textProperty().addListener((obs, oldIme, newIme) -> {
            if (!newIme.isEmpty()) {
                fldIme.getStyleClass().removeAll("poljeNijeIspravno");
                fldIme.getStyleClass().add("poljeIspravno");
            } else {
                fldIme.getStyleClass().removeAll("poljeIspravno");
                fldIme.getStyleClass().add("poljeNijeIspravno");
            }
        });

        fldPrezime.textProperty().addListener((obs, oldIme, newIme) -> {
            if (!newIme.isEmpty()) {
                fldPrezime.getStyleClass().removeAll("poljeNijeIspravno");
                fldPrezime.getStyleClass().add("poljeIspravno");
            } else {
                fldPrezime.getStyleClass().removeAll("poljeIspravno");
                fldPrezime.getStyleClass().add("poljeNijeIspravno");
            }
        });

        fldEmail.textProperty().addListener((obs, oldIme, newIme) -> {
            if (!newIme.isEmpty()) {
                fldEmail.getStyleClass().removeAll("poljeNijeIspravno");
                fldEmail.getStyleClass().add("poljeIspravno");
            } else {
                fldEmail.getStyleClass().removeAll("poljeIspravno");
                fldEmail.getStyleClass().add("poljeNijeIspravno");
            }
        });

        fldUsername.textProperty().addListener((obs, oldIme, newIme) -> {
            if (!newIme.isEmpty()) {
                fldUsername.getStyleClass().removeAll("poljeNijeIspravno");
                fldUsername.getStyleClass().add("poljeIspravno");
            } else {
                fldUsername.getStyleClass().removeAll("poljeIspravno");
                fldUsername.getStyleClass().add("poljeNijeIspravno");
            }
        });

        fldPassword.textProperty().addListener((obs, oldIme, newIme) -> {
            if (!newIme.isEmpty()) {
                fldPassword.getStyleClass().removeAll("poljeNijeIspravno");
                fldPassword.getStyleClass().add("poljeIspravno");
            } else {
                fldPassword.getStyleClass().removeAll("poljeIspravno");
                fldPassword.getStyleClass().add("poljeNijeIspravno");
            }
        });
    }

    public void dodajAction(ActionEvent actionEvent) {
        int id=model.noviId();
        model.dodaj(id);
        model.getKorisnici().add(new Korisnik("", "", "", "", ""));
        model.getKorisnici().get(model.getKorisnici().size()-1).setId(id);
        listKorisnici.getSelectionModel().selectLast();
    }

    public void krajAction(ActionEvent actionEvent) {
        System.exit(0);
    }

    public void obrisiAction(ActionEvent actionEvent) {
        model.obrisi();
        model.setTrenutniKorisnik(null);
        listKorisnici.refresh();
    }

    public void aboutAction(ActionEvent actionEvent) {
        if(!eng) {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("Informacije o aplikaciji");
            alert.setHeaderText("O aplikaciji");
            GridPane gridPane=new GridPane();
            gridPane.addColumn(1);
            gridPane.addRow(1,new Label("Verzija: 1.3.2\n" +
                    "Github repozitorij: https://github.com/RPR-2019/rpr21-zadaca4-apandzic2\n" +
                    "Autor: Amina Pandžić"));
            gridPane.addRow(2,new ImageView("/img/pic.jpg"));
            gridPane.setHgap(5);
            gridPane.setVgap(5);
            gridPane.setPadding(new Insets(4,4,4,4));
            alert.getDialogPane().setContent(gridPane);
            alert.showAndWait();
        } else {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("About the application");
            alert.setHeaderText("About");
            GridPane gridPane=new GridPane();
            gridPane.addColumn(1);
            gridPane.addRow(1,new Label("Version: 1.3.2\n" +
                    "Github repository: https://github.com/RPR-2019/rpr21-zadaca4-apandzic2\n" +
                    "Author: Amina Pandžić"));
            gridPane.addRow(2,new ImageView("/img/pic.jpg"));
            gridPane.setHgap(5);
            gridPane.setVgap(5);
            gridPane.setPadding(new Insets(4,4,4,4));
            alert.getDialogPane().setContent(gridPane);
            alert.showAndWait();
        }
    }

    public void saveAction(ActionEvent actionEvent) {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Unesite naziv datoteke");
        fileChooser.getExtensionFilters().addAll(new FileChooser.ExtensionFilter("TXT datoteke", "*.txt"),new FileChooser.ExtensionFilter("WORD datoteka","*.docx","*.doc","*.rtf"));
        File file = fileChooser.showSaveDialog(fldIme.getScene().getWindow());
        if (file != null) {
            model.zapisiDatoteku(file);
        }
    }

    public void printAction(ActionEvent actionEvent) {
        try {
            new PrintReport().showReport(model.getConn());
        } catch (JRException e1) {
            e1.printStackTrace();
        }
    }

    public void bosanskiAction(ActionEvent actionEvent){
        eng=false;
        LoadView(new Locale("bs","BA"));
    }

    public void engleskiAction(ActionEvent actionEvent){
        eng=true;
        LoadView(new Locale("en","US"));
    }
    
    public void LoadView(Locale locale) {
        Scene s= fldIme.getScene();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/korisnici.fxml"));
        loader.setResources(ResourceBundle.getBundle("Translation",locale));
        loader.setController(this);
        try {
            Parent root = loader.load();
            s.setRoot(root);
            Stage stage=(Stage)s.getWindow();
            if(eng) {
                stage.setTitle("Users");
            } else {
                stage.setTitle("Korisnici");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
