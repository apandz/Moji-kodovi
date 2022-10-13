package ba.unsa.etf.rpr.t7;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

import java.io.File;

import static org.junit.jupiter.api.Assertions.*;

class KorisniciModelTest {
    @Test
    void konstruktor() {
        // Konstruktor ne treba da popuni model, atributi su prazni
        KorisniciModel m = new KorisniciModel();
        assertNull(m.getTrenutniKorisnik());
        assertTrue(m.getKorisnici().isEmpty());
    }

    @Test
    void napuni() {
        // Metoda napuni popunjava podatke iz tutorijala
        KorisniciModel m = new KorisniciModel();
        // Pozivamo metodu vratiNaDefault koja trenutno ne radi ništa, a kasnije će osiguravati
        // da testovi ne zavise jedan od drugog, tako da se mogu pokretati svi odjednom umjesto jedan po jedan
        m.vratiNaDefault();
        // Sada punimo default podatke u atribute
        m.napuni();
        assertEquals(4, m.getKorisnici().size());
        assertNull(m.getTrenutniKorisnik());
    }

    @Test
    void napuni2() {
        KorisniciModel m = new KorisniciModel();
        m.vratiNaDefault(); // Kao i ranije
        m.napuni();
        String s = "";
        for(Korisnik k : m.getKorisnici())
            s += k;
        String expected = "Ljubović VedranDelić AmraSijerčić TarikFejzić Rijad";
        assertEquals(expected, s);
    }

    @Test
    void trenutniKorisnik() {
        KorisniciModel m = new KorisniciModel();
        m.vratiNaDefault(); // Kao i ranije
        m.napuni();
        m.setTrenutniKorisnik(m.getKorisnici().get(0));
        String s = "" + m.getTrenutniKorisnik();
        assertEquals("Ljubović Vedran", s);
    }


    @Test
    void promijeniTrenutniKorisnik() {
        KorisniciModel m = new KorisniciModel();
        m.vratiNaDefault(); // Kao i ranije
        m.napuni();
        m.setTrenutniKorisnik(m.getKorisnici().get(2));
        m.getTrenutniKorisnik().setIme("aaaaa");
        assertEquals("aaaaa", m.getKorisnici().get(2).getIme());
    }}