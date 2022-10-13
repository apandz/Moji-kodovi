package ba.unsa.etf.rpr;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;
import net.sf.jasperreports.engine.JRException;

import java.io.IOException;
import java.util.Locale;
import java.util.Optional;
import java.util.ResourceBundle;

import static javafx.scene.layout.Region.USE_COMPUTED_SIZE;

public class GlavnaController {
    private int jezik;
    public TableView tableViewGradovi;
    public TableColumn colGradId,colGradNaziv,colGradStanovnika,colGradDrzava,colGradPostanskiBroj;
    public Button btnDodajGrad, btnDodajDrzavu, btnIzmijeniGrad, btnObrisiGrad;

    private ObservableList<Grad> gradovi;

    public GlavnaController() {
        jezik=1;
    }

    @FXML
    public void initialize() {
        gradovi= FXCollections.observableArrayList(GeografijaDAO.getInstance().gradovi());
        colGradId.setCellValueFactory(new PropertyValueFactory<Grad,Integer>("id"));
        colGradNaziv.setCellValueFactory(new PropertyValueFactory<Grad,String>("naziv"));
        colGradStanovnika.setCellValueFactory(new PropertyValueFactory<Grad,Integer>("brojStanovnika"));
        colGradDrzava.setCellValueFactory(new PropertyValueFactory<Grad,String>("drzava"));
        colGradPostanskiBroj.setCellValueFactory(new PropertyValueFactory<Grad,Integer>("postanskiBroj"));
        tableViewGradovi.setItems(gradovi);
    }

    public void dodajGradAction(ActionEvent actionEvent) throws Exception {
        Stage novi=new Stage();
        Locale locale=new Locale("bs");
        if(jezik==2) {
            locale=new Locale("en","US");
        } else if(jezik==3) {
            locale=new Locale("de","DE");
        } else if(jezik==4) {
            locale=new Locale("fr","FR");
        }
        ResourceBundle bundle = ResourceBundle.getBundle("Translation",locale);
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/grad.fxml"),bundle);
        GradController ctrl = new GradController(null, GeografijaDAO.getInstance().drzave());
        ctrl.setJezik(jezik);
        loader.setController(ctrl);
        Parent root = loader.load();
        novi.setTitle("Grad");
        if(jezik==2) {
           novi.setTitle("City");
        } else if(jezik==3) {
            novi.setTitle("Stadt");
        } else if(jezik==4) {
            novi.setTitle("Ville");
        }
        novi.setScene(new Scene(root, USE_COMPUTED_SIZE, USE_COMPUTED_SIZE));
        novi.setResizable(false);
        novi.show();
        novi.toFront();
        novi.setOnHiding((o)->{
            Grad g= ctrl.getGrad();
            if(g!=null) {
                GeografijaDAO.getInstance().dodajGrad(g);
                gradovi.add(g);
                tableViewGradovi.refresh();
            }
        });
    }

    public void dodajDrzavuAction(ActionEvent actionEvent) throws Exception {
        Stage novi=new Stage();
        Locale locale=new Locale("bs");
        if(jezik==2) {
            locale=new Locale("en","US");
        } else if(jezik==3) {
            locale=new Locale("de","DE");
        } else if(jezik==4) {
            locale=new Locale("fr","FR");
        }
        ResourceBundle bundle = ResourceBundle.getBundle("Translation",locale);
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/drzava.fxml"),bundle);
        DrzavaController ctrl = new DrzavaController(null,GeografijaDAO.getInstance().gradovi());
        loader.setController(ctrl);
        Parent root = loader.load();
        novi.setTitle("Država");
        if(jezik==2) {
            novi.setTitle("Country");
        } else if(jezik==3) {
            novi.setTitle("Land");
        } else if(jezik==4) {
            novi.setTitle("Pays");
        }
        novi.setScene(new Scene(root, USE_COMPUTED_SIZE, USE_COMPUTED_SIZE));
        novi.setResizable(false);
        novi.show();
        novi.toFront();
        novi.setOnHiding((o)->{
            Drzava d= ctrl.getDrzava();
            if(d!=null) {
                GeografijaDAO.getInstance().dodajDrzavu(d);
                tableViewGradovi.refresh();
            }
        });
    }

    public void izmijeniGradAction(ActionEvent actionEvent) throws Exception {
        Stage novi=new Stage();
        Locale locale=new Locale("bs");
        if(jezik==2) {
            locale=new Locale("en","US");
        } else if(jezik==3) {
            locale=new Locale("de","DE");
        } else if(jezik==4) {
            locale=new Locale("fr","FR");
        }
        ResourceBundle bundle = ResourceBundle.getBundle("Translation",locale);
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/grad.fxml"),bundle);
        Grad g=(Grad)tableViewGradovi.getSelectionModel().getSelectedItem();
        if(g!=null) {
            String stariNaziv=g.getNaziv(), staraSlika=g.getSlika();
            int stariBrojStanovnika=g.getBrojStanovnika(), staraDrzava=g.getDrzava().getId(), stariPostanskiBroj=g.getPostanskiBroj();
            GradController ctrl = new GradController(g, GeografijaDAO.getInstance().drzave());
            loader.setController(ctrl);
            Parent root = loader.load();
            novi.setTitle("Grad");
            if(jezik==2) {
                novi.setTitle("City");
            } else if(jezik==3) {
                novi.setTitle("Stadt");
            } else if(jezik==4) {
                novi.setTitle("Ville");
            }
            novi.setScene(new Scene(root, USE_COMPUTED_SIZE, USE_COMPUTED_SIZE));
            novi.setResizable(false);
            novi.show();
            novi.toFront();
            novi.setOnHiding((o) -> {
                if (g != null && (!g.getNaziv().equals(stariNaziv) || g.getBrojStanovnika()!=stariBrojStanovnika || (g.getDrzava()!=null && g.getDrzava().getId()!=staraDrzava) || (g.getSlika()==null && staraSlika!=null) ||(g.getSlika()!=null &&!g.getSlika().equals(staraSlika)) || g.getPostanskiBroj()!=stariPostanskiBroj)) {
                    GeografijaDAO.getInstance().izmijeniGrad(g);
                    gradovi.remove((Grad) tableViewGradovi.getSelectionModel().getSelectedItem());
                    gradovi.add(g);
                    tableViewGradovi.refresh();
                }
            });
        }
    }

    public void obrisiGradAction(ActionEvent actionEvent) throws Exception {
        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Brisanje grada");
        Grad g=(Grad)tableViewGradovi.getSelectionModel().getSelectedItem();
        if(g!=null) {
            alert.setHeaderText("Brisanje grada " + g.getNaziv());
            alert.setContentText("Da li želite obrisati ovaj grad?");
            if (jezik == 2) {
                alert.setTitle("Delete city");
                alert.setHeaderText("Delete city " + g.getNaziv());
                alert.setContentText("Do you want to delete this city?");
            } else if (jezik == 3) {
                alert.setTitle("Stadt löschen");
                alert.setHeaderText("Stadt " + g.getNaziv() + " löschen");
                alert.setContentText("Möchten Sie diese Stadt löschen?");
            } else if (jezik == 4) {
                alert.setTitle("Supprimer la ville");
                alert.setHeaderText("Supprimer la ville " + g.getNaziv());
                alert.setContentText("Voulez-vous supprimer cette ville?");
            }
            Optional<ButtonType> result = alert.showAndWait();
            if (result.isPresent() && result.get() == ButtonType.OK) {
                GeografijaDAO.getInstance().obrisiGrad(g.getId());
                gradovi.remove(g);
            }
        }
    }

    public void printReport(ActionEvent actionEvent) {
        try {
            new GradoviReport().showReport(GeografijaDAO.getInstance().getConn(),jezik);
        } catch (JRException e1) {
            e1.printStackTrace();
        }
    }

    public void jezikAction(ActionEvent actionEvent) {
        String def="Bosanski",bosanski="Bosanski",engleski="Engleski",njemacki="Njemački",francuski="Francuski";
        if(jezik==2) {
            def="English";
            bosanski="Bosnian";
            engleski="English";
            njemacki="German";
            francuski="French";
        } else if(jezik==3) {
            def="Deutsch";
            bosanski="Bosnisch";
            engleski="Englisch";
            njemacki="Deutsch";
            francuski="Französisch";
        } else if(jezik==4) {
            def="Français";
            bosanski="Bosniaque";
            engleski="Anglais";
            njemacki="Allemand";
            francuski="Français";
        }
        ChoiceDialog choiceDialog=new ChoiceDialog(def,bosanski,engleski,njemacki,francuski);
        choiceDialog.setContentText("Izaberite jezik:");
        choiceDialog.setTitle("Potvrda");
        choiceDialog.setHeaderText("Jezik");
        if(jezik==2) {
            choiceDialog.setContentText("Choose a language:");
            choiceDialog.setTitle("Confirmation");
            choiceDialog.setHeaderText("Language");
        } else if(jezik==3) {
            choiceDialog.setContentText("Wählen Sie eine Sprache:");
            choiceDialog.setTitle("Bestätigung");
            choiceDialog.setHeaderText("Sprache");
        } else if(jezik==4) {
            choiceDialog.setContentText("Choisissez une langue:");
            choiceDialog.setTitle("Confirmation");
            choiceDialog.setHeaderText("Langue");
        }
        Optional<String> result = choiceDialog.showAndWait();
        if (result.isPresent()) {
            if (result.get().equals(bosanski) && !def.equals(bosanski)) {
                bosanskiAction();
            } else if (result.get().equals(engleski) && !def.equals(engleski)) {
                engleskiAction();
            } else if (result.get().equals(njemacki) && !def.equals(njemacki)) {
                njemackiAction();
            } else if (result.get().equals(francuski) && !def.equals(francuski)) {
                francuskiAction();
            }
        }
    }

    public void bosanskiAction(){
        jezik=1;
        LoadView(new Locale("bs"));
    }

    public void engleskiAction(){
        jezik=2;
        LoadView(new Locale("en","US"));
    }

    public void njemackiAction(){
        jezik=3;
        LoadView(new Locale("de","DE"));
    }

    public void francuskiAction(){
        jezik=4;
        LoadView(new Locale("fr","FR"));
    }

    public void LoadView(Locale locale) {
        Scene s= btnDodajGrad.getScene();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/glavna.fxml"));
        loader.setResources(ResourceBundle.getBundle("Translation",locale));
        loader.setController(this);
        try {
            Parent root = loader.load();
            s.setRoot(root);
            Stage stage=(Stage)s.getWindow();
            if(jezik==2){
                stage.setTitle("Cities of the world");
            } else if(jezik==3) {
                stage.setTitle("Städte der Welt");
            } else if(jezik==4) {
                stage.setTitle("Villes du monde");
            } else {
                stage.setTitle("Gradovi svijeta");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
