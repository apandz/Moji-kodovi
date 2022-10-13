package ba.unsa.etf.rpr.t7;

import org.junit.jupiter.api.Test;

import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

import static org.junit.jupiter.api.Assertions.*;

// Svrha klase Zadatak1TestModel je da provjeri da li Model klasa kreira bazu podataka kako je traženo u zadatku
public class Zadatak1TestModel {
    @Test
    void praznaBaza() {
        // Brišemo fajl korisnici.db
        File dbfile = new File("korisnici.db");
        dbfile.delete();

        // Pozivamo konstruktor modela
        KorisniciModel model = new KorisniciModel();
        model.diskonektuj();

        // Da li fajl sada postoji?
        assertTrue(dbfile.exists());
    }

    @Test
    void validnaBaza() {
        // Brišemo fajl korisnici.db
        File dbfile = new File("korisnici.db");
        dbfile.delete();

        // Pozivamo konstruktor modela
        KorisniciModel model = new KorisniciModel();
        model.diskonektuj();

        // Da li baza sadrži defaultne podatke (tabela korisnik)
        Connection conn;
        try {
            conn = DriverManager.getConnection("jdbc:sqlite:korisnici.db");
            try {
                PreparedStatement korisnikUpit = conn.prepareStatement("SELECT * FROM korisnik");
                korisnikUpit.execute();
                conn.close();
            } catch (SQLException e) {
                fail("Tabela korisnik ne postoji u bazi");
            }
        } catch (SQLException e) {
            fail("Datoteka sa bazom ne postoji ili je nedostupna");
        }
    }

    @Test
    void testPromjenaBaze() {
        // Provjera da li izmjena podatka u modelu ostaje trajno (perzistira)
        {
            KorisniciModel model = new KorisniciModel();
            model.vratiNaDefault();
            model.napuni();
            Korisnik prvi = model.getKorisnici().get(0);
            Korisnik drugi = model.getKorisnici().get(1);
            model.setTrenutniKorisnik(prvi);
            prvi.setIme("Test");
            // Promjena trenutnog korisnika bi trebala izvršiti ažuriranje baze
            model.setTrenutniKorisnik(drugi);
            model.diskonektuj();
        }
        // Pošto koristimo vitičaste zagrade, kompletan objekat "model" je uništen i sada kreiramo novi
        {
            KorisniciModel model = new KorisniciModel();
            model.napuni();
            Korisnik prvi = model.getKorisnici().get(0);
            assertEquals("Test", prvi.getIme());
        }
    }
}
