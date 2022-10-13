package ba.unsa.etf.rpr;

import javafx.application.Platform;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.scene.control.TextInputControl;
import javafx.scene.control.TextInputDialog;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.stage.Stage;

import java.io.*;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Locale;
import java.util.Optional;
import java.util.ResourceBundle;

import static javafx.scene.layout.Region.USE_COMPUTED_SIZE;

public class GradController {
    public TextField fieldNaziv, fieldBrojStanovnika,fldPostanskiBroj;
    public ChoiceBox<Drzava> choiceDrzava;
    public ImageView imgView;
    private SimpleStringProperty naziv, brojStanovnika, postanskiBroj;
    private Grad gradZaIzmjenu;
    private String imgPath;

    private boolean ok,pst;
    private int jezik;

    private ObservableList<Drzava> drzave;

    public GradController(Grad grad, ArrayList<Drzava> drzave) {
        naziv=new SimpleStringProperty("");
        brojStanovnika=new SimpleStringProperty("");
        postanskiBroj=new SimpleStringProperty("");
        this.drzave= FXCollections.observableArrayList(drzave);
        ok=false;
        gradZaIzmjenu=grad;
        jezik=1;
        if(gradZaIzmjenu!=null){
            imgPath=gradZaIzmjenu.getSlika();
        } else {
            imgPath="";
        }
        pst=false;
    }

    public void setJezik(int jezik) {
        this.jezik=jezik;
    }

    @FXML
    public void initialize() {
        fieldNaziv.textProperty().bindBidirectional(naziv);
        fieldBrojStanovnika.textProperty().bindBidirectional(brojStanovnika);
        fldPostanskiBroj.textProperty().bindBidirectional(postanskiBroj);
        choiceDrzava.setItems(drzave);
        if(gradZaIzmjenu!=null) {
            naziv=new SimpleStringProperty(gradZaIzmjenu.getNaziv());
            fieldNaziv.setText(naziv.getValue());
            brojStanovnika=new SimpleStringProperty(gradZaIzmjenu.getBrojStanovnika()+"");
            fieldBrojStanovnika.setText(brojStanovnika.getValue());
            postanskiBroj=new SimpleStringProperty(gradZaIzmjenu.getPostanskiBroj()+"");
            fldPostanskiBroj.setText(postanskiBroj.getValue());
            Drzava dd=null;
            for(Drzava d: drzave) {
                if(d.getNaziv().equals(gradZaIzmjenu.getDrzava().getNaziv())) {
                    dd=d;
                    break;
                }
            }
            choiceDrzava.getSelectionModel().select(dd);
            if(gradZaIzmjenu.getSlika()!=null) {
                File file = new File(gradZaIzmjenu.getSlika());
                if (file.exists()) {
                    try {
                        imgPath = gradZaIzmjenu.getSlika();
                        FileInputStream fileInputStream = new FileInputStream(imgPath);
                        imgView.setImage(new Image(fileInputStream));

                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    public void zatvoriProzor(ActionEvent actionEvent) {
        Node n = (Node) actionEvent.getSource();
        Stage stage = (Stage) n.getScene().getWindow();
        stage.close();
    }

    public void validiraj(ActionEvent actionEvent) {
        boolean nazivT=false, brStT=false;
        if(fieldNaziv.getText().trim().isEmpty()) {
            fieldNaziv.getStyleClass().removeAll("poljeJeIspravno");
            fieldNaziv.getStyleClass().add("poljeNijeIspravno");
        } else {
            fieldNaziv.getStyleClass().removeAll("poljeNijeIspravno");
            fieldNaziv.getStyleClass().add("poljeJeIspravno");
            nazivT=true;
            naziv.setValue(fieldNaziv.getText());
        } try{
            if(fieldBrojStanovnika.getText().trim().isEmpty() || Integer.parseInt(brojStanovnika.getValue())<=0) {
                fieldBrojStanovnika.getStyleClass().removeAll("poljeJeIspravno");
                fieldBrojStanovnika.getStyleClass().add("poljeNijeIspravno");
            } else {
                fieldBrojStanovnika.getStyleClass().removeAll("poljeNijeIspravno");
                fieldBrojStanovnika.getStyleClass().add("poljeJeIspravno");
                brStT=true;
                brojStanovnika.set(fieldBrojStanovnika.getText());
            }
        }catch (NumberFormatException exception) {
            fieldBrojStanovnika.getStyleClass().removeAll("poljeJeIspravno");
            fieldBrojStanovnika.getStyleClass().add("poljeNijeIspravno");
        }
        new Thread(() -> {
            try {int postanskiBroj1 = Integer.parseInt(fldPostanskiBroj.getText());
                    try {

                        URL url = new URL("http://c9.etf.unsa.ba/proba/postanskiBroj.php?postanskiBroj=" + postanskiBroj1);
                        BufferedReader ulaz = new BufferedReader(new InputStreamReader(url.openStream()));
                        String json = "", line = null;
                        while ((line = ulaz.readLine()) != null)
                            json = json + line;
                        if (json.equals("NOT OK")) {
                            Platform.runLater(()-> { fldPostanskiBroj.getStyleClass().removeAll("poljeJeIspravno");
                            fldPostanskiBroj.getStyleClass().add("poljeNijeIspravno");});
                        } else if (json.equals("OK")) {
                            pst = true;
                            Platform.runLater(()-> { fldPostanskiBroj.getStyleClass().removeAll("poljeNijeIspravno");
                            fldPostanskiBroj.getStyleClass().add("poljeJeIspravno");});
                            if (gradZaIzmjenu != null) {
                                gradZaIzmjenu.setNaziv(naziv.getValue());
                                gradZaIzmjenu.setBrojStanovnika(Integer.parseInt(brojStanovnika.getValue()));
                                gradZaIzmjenu.setDrzava((Drzava) choiceDrzava.getSelectionModel().getSelectedItem());
                                gradZaIzmjenu.setSlika(imgPath);
                                gradZaIzmjenu.setPostanskiBroj(postanskiBroj1);
                            }
                            ok = true;
                            Platform.runLater(()-> { Stage stage = (Stage) fieldNaziv.getScene().getWindow();
                            if(stage!=null)stage.close();
                        });

                        }

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
            } catch(NumberFormatException ne) {
                Platform.runLater(()-> { fldPostanskiBroj.getStyleClass().removeAll("poljeJeIspravno");
                    fldPostanskiBroj.getStyleClass().add("poljeNijeIspravno");});
            }
        }).start();
        if(nazivT && brStT && pst)  {
            if(gradZaIzmjenu!=null) {
                gradZaIzmjenu.setNaziv(naziv.getValue());
                gradZaIzmjenu.setBrojStanovnika(Integer.parseInt(brojStanovnika.getValue()));
                gradZaIzmjenu.setDrzava((Drzava)choiceDrzava.getSelectionModel().getSelectedItem());
                gradZaIzmjenu.setSlika(imgPath);
                gradZaIzmjenu.setPostanskiBroj(Integer.parseInt(postanskiBroj.getValue()));
            }
            ok=true;
            zatvoriProzor(actionEvent);
        }
    }

    public Grad getGrad() {
        if(ok){
            Grad grad=new Grad(0,naziv.getValue(),Integer.parseInt(brojStanovnika.getValue()),(Drzava)choiceDrzava.getValue());
            grad.setSlika(imgPath);
            grad.setPostanskiBroj(Integer.parseInt(fldPostanskiBroj.getText()));
            return grad;
        }
        return null;
    }

    public void promijeniSlikuAction(ActionEvent actionEvent) throws Exception {
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
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/pretraga.fxml"),bundle);
        PretragaController ctrl = new PretragaController();
        loader.setController(ctrl);
        Parent root = loader.load();
        novi.setTitle("Pretraga datoteke");
        if(jezik==2) {
            novi.setTitle("File search");
        } else if(jezik==3) {
            novi.setTitle("Dateisuche");
        } else if(jezik==4) {
            novi.setTitle("Recherche de fichier");
        }
        novi.setScene(new Scene(root, USE_COMPUTED_SIZE, USE_COMPUTED_SIZE));
        novi.setResizable(false);
        novi.show();
        novi.toFront();
        novi.setOnHiding((o)->{
            String s=ctrl.puteviView.getSelectionModel().getSelectedItem();
            if(s!=null) {
                imgPath=s;
                File file = new File(imgPath);
                if (file.exists()) {
                    try {
                        FileInputStream fileInputStream = new FileInputStream(imgPath);
                        imgView.setImage(new Image(fileInputStream));
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
    }
}
