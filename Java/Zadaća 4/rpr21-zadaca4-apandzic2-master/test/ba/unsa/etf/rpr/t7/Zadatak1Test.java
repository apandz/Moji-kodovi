package ba.unsa.etf.rpr.t7;

import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.testfx.api.FxRobot;
import org.testfx.framework.junit5.ApplicationExtension;
import org.testfx.framework.junit5.Start;

import java.io.File;
import java.sql.*;
import java.util.ResourceBundle;

import static javafx.scene.layout.Region.USE_COMPUTED_SIZE;
import static org.junit.jupiter.api.Assertions.*;

// Svrha ove klase je da se uvjeri da se promjene na grafičkom korisničkom interfejsu odražavaju na bazu
// i obrnuto
@ExtendWith(ApplicationExtension.class)
public class Zadatak1Test {
    KorisniciModel model;

    @Start
    public void start (Stage stage) throws Exception {
        model = new KorisniciModel();
        model.vratiNaDefault();
        model.napuni();
        KorisnikController ctrl = new KorisnikController(model);
        ResourceBundle bundle = ResourceBundle.getBundle("Translation");
        FXMLLoader loader = new FXMLLoader(getClass().getResource("/fxml/korisnici.fxml"),bundle);
        loader.setController(ctrl);
        Parent root = loader.load();
        stage.setTitle("Korisnici");
        stage.setScene(new Scene(root, USE_COMPUTED_SIZE, USE_COMPUTED_SIZE));
        stage.show();
        stage.toFront();
    }

    @Test
    void testBazaGui(FxRobot robot) {
        robot.clickOn("Sijerčić Tarik");
        robot.clickOn("#fldIme").write("bbb");
        robot.clickOn("Fejzić Rijad");
        model.diskonektuj();

        // Kreiramo novu konekciju na bazu
        Connection conn;
        try {
            conn = DriverManager.getConnection("jdbc:sqlite:korisnici.db");
            try {
                PreparedStatement korisnikUpit = conn.prepareStatement("SELECT ime FROM korisnik WHERE prezime='Sijerčić'");
                ResultSet rs = korisnikUpit.executeQuery();
                rs.next();
                String ime = rs.getString(1);
                assertEquals("Tarikbbb", ime);
                conn.close();
            } catch (SQLException e) {
                fail("Nije uspio upit na korisnika sa prezimenom 'Sijerčić'");
            }
        } catch (SQLException e) {
            fail("Datoteka sa bazom ne postoji ili je nedostupna");
        }
    }
}
