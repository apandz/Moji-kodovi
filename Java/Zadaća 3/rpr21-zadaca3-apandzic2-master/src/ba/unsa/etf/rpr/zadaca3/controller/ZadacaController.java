package ba.unsa.etf.rpr.zadaca3.controller;

import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.ListView;
import javafx.scene.control.Slider;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import static javafx.scene.layout.Region.USE_COMPUTED_SIZE;

public class ZadacaController {
    public TextField fldText;
    public Slider sliderStudents;
    public ListView<String> lvStudents;
    public ChoiceBox<String> choiceColor;
    private SimpleStringProperty text;
    private SimpleDoubleProperty slider;
    private ObservableList<String> students;
    private ObservableList<String> colors;
    private NoviController noviController;

    public ZadacaController() {
        text=new SimpleStringProperty("");
        slider=new SimpleDoubleProperty(5);
        students= FXCollections.observableArrayList();
        for(int i=1; i<6; i++) {
            students.add("Student" + i);
        }
        colors= FXCollections.observableArrayList();
        colors.add("Default");
        colors.add("Crvena");
        colors.add("Zelena");
        colors.add("Plava");
        noviController=new NoviController();
    }

    @FXML
    public void initialize() {
        fldText.textProperty().bindBidirectional(text);
        lvStudents.setItems(students);
        choiceColor.setItems(colors);
        sliderStudents.valueProperty().addListener((obs,oldS,newS) -> {
            double noviSlider1=(double)newS;
            double stariSlider1=(double)oldS;
            int noviSlider=(int)noviSlider1;
            int stariSlider=(int)stariSlider1;
            if(noviSlider!=stariSlider) {
                if (noviSlider < stariSlider) {
                    for(int i=stariSlider-1; i>=noviSlider; i--) {
                        students.remove(i);
                    }
                } else {
                    if(students.size()<15) {
                        for (int i = stariSlider; i < noviSlider; i++) {
                            if (i == 14) {
                                students.add("Student" + text.get());
                            } else {
                                students.add("Student" + (i + 1));
                            }
                        }
                    }
                }
                slider.setValue(noviSlider);
            }
            lvStudents.setItems(students);
            lvStudents.refresh();
        });
    }

    private boolean manjaVr() {
        if(!text.get().isBlank()) {
            if (Integer.parseInt(text.get()) > 1000000) {
                text.set("");
                return false;
            }
        }
        return true;
    }

    public void btn0Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get() + "0");
        }
    }

    public void btn1Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get() + "1");
        }
    }

    public void btn2Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"2");
        }
    }

    public void btn3Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"3");
        }
    }

    public void btn4Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"4");
        }
    }

    public void btn5Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"5");
        }
    }

    public void btn6Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"6");
        }
    }

    public void btn7Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"7");
        }
    }

    public void btn8Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"8");
        }
    }

    public void btn9Click(ActionEvent actionEvent) {
        if(manjaVr()) {
            text.set(text.get()+"9");
        }
    }

    public void izabranaBoja(ActionEvent actionEvent) {
        Node node = (Node) actionEvent.getSource();
        Scene scene = node.getScene();
        scene.getStylesheets().clear();
        String novaBoja=choiceColor.getValue();
        if(novaBoja.equals("Crvena"))
            scene.getStylesheets().add("/css/crvena.css");
        else if(novaBoja.equals("Zelena"))
            scene.getStylesheets().add("/css/zelena.css");
        else if(novaBoja.equals("Plava"))
            scene.getStylesheets().add("/css/plava.css");
    }

    public void otvoriNovi(ActionEvent actionEvent) throws Exception {
        Stage myStage = new Stage();
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/novi.fxml"));
        loader.load();
        noviController = loader.getController();
        myStage.setTitle("Unos studenta");
        myStage.setScene(new Scene(loader.getRoot(), USE_COMPUTED_SIZE, USE_COMPUTED_SIZE));
        myStage.setResizable(false);
        myStage.show();
        myStage.setOnHiding((value)-> {
            if(noviController.validnoIme) {
                if(students.size()<15) {
                    students.add(noviController.fldIme.getText());
                    slider.setValue(slider.intValue()+1);
                    lvStudents.setItems(students);
                    lvStudents.refresh();
                }
            }
        });
    }

}
