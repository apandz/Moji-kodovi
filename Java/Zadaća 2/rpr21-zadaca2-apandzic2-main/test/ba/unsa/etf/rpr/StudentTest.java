package ba.unsa.etf.rpr;

import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class StudentTest {
    private Fakultet fakultet;
    private Studij studij;

    StudentTest()  {
        fakultet = new Fakultet("Elektrotehnički fakultet Sarajevo");
        studij=new Studij("Prvi ciklus studija",3);
        Semestar semestar1=new Semestar("Prvi semestar");
        semestar1.dodajObavezanPredmet(new Predmet("Inženjerska matematika 1",7,49));
        semestar1.dodajObavezanPredmet(new Predmet("Inženjerska fizika 1",5,39));
        semestar1.dodajObavezanPredmet(new Predmet("Osnove elektrotehnike",7,48));
        semestar1.dodajObavezanPredmet(new Predmet("Linearna algebra i geometrija",5,39));
        semestar1.dodajObavezanPredmet(new Predmet("Uvod u programiranje",6,44));
        Semestar semestar3=new Semestar("Treći semestar");
        semestar3.dodajObavezanPredmet(new Predmet("Algoritmi i strukture podataka",5,45));
        semestar3.dodajObavezanPredmet(new Predmet("Logički dizajn",5,38));
        semestar3.dodajObavezanPredmet(new Predmet("Razvoj programskih rješenja",5,38));
        semestar3.dodajObavezanPredmet(new Predmet("Osnove baza podataka",5,45));
        semestar3.dodajObavezanPredmet(new Predmet("Diskretna matematika",5,39));
        semestar3.dodajIzborniPredmet(new Predmet("Numerički algoritmi",5,35));
        semestar3.dodajIzborniPredmet(new Predmet("Sistemsko programiranje",5,35));
        studij.dodajSemestar("AE",semestar1);
        studij.dodajSemestar("EE",semestar1);
        studij.dodajSemestar("RI",semestar1);
        studij.dodajSemestar("TK",semestar1);
        studij.dodajSemestar("RI",semestar3);
        fakultet.dodajStudij(studij);
    }

    @Test
    void getIme() {
        Student student = new Student("Azra","Azrić",fakultet,studij,"RI");
        assertEquals("Azra",student.getIme());
    }

    @Test
    void getPrezime() {
        Student student = new Student("Ana","Marković",fakultet,studij,"RI");
        assertEquals("Marković",student.getPrezime());
    }

    @Test
    void getIndeks() {
        Student student = new Student("Marko","Anić",fakultet,studij,"TK");
        String indeks="Ime i prezime studenta: "+student.getIme()+" "+student.getPrezime()+"\nFakultet: "+student.getIndeks().getFakultet().getNaziv()+"\nStudij: "+student.getIndeks().getStudij().getNaziv()+"\nSmjer: "+student.getIndeks().getSmjer();
        String indeks1="Ime i prezime studenta: Marko Anić\nFakultet: Elektrotehnički fakultet Sarajevo\nStudij: Prvi ciklus studija\nSmjer: TK";
        assertEquals(indeks1,indeks);
    }

    @Test
    void promijeniIndeks1() {
        Student student = new Student("Ajna","Hadžić",fakultet,studij,"AE");
        int indeks =student.getIndeks().getBrojIndeksa();
        student.promijeniIndeks(student.getIndeks().getFakultet(), new Studij("Drugi ciklus studija",2),"AE");
        assertEquals(indeks, student.getIndeks().getBrojIndeksa());
    }

    @Test
    void promijeniIndeks2() {
        Student student = new Student("Ajla","Hodžić",new Fakultet("Prirodno-matematički fakultet Sarajevo"),new Studij("Prvi ciklus studija",3),"TKN");
        student.promijeniIndeks(fakultet, studij,"RI");
        assertEquals(fakultet,student.getIndeks().getFakultet());
        assertEquals(studij,student.getIndeks().getStudij());
        assertEquals("RI", student.getIndeks().getSmjer());
    }

    @Test
    void upisiSemestar1() {
        Student student = new Student("Emir","Hodžić",fakultet,studij,"RI");
        assertTrue(student.upisiSemestar(studij.getSemestri("RI").get(0),new ArrayList<>()));
    }
    @Test
    void upisiSemestar2() {
        Student student = new Student("Emir","Hodžić",fakultet,studij,"RI");
        List<Predmet> izborniPredmeti=new ArrayList<>();
        izborniPredmeti.add(new Predmet("Numerički algoritmi",5,35));
        assertTrue(student.upisiSemestar(studij.getSemestri("RI").get(1),izborniPredmeti));
    }

    @Test
    void upisiSemestar3() {
        Student student = new Student("Emir","Hodžić",fakultet,studij,"RI");
        List<Predmet> izborniPredmeti=new ArrayList<>();
        izborniPredmeti.add(new Predmet("Numerički algoritmi",5,35));
        izborniPredmeti.add(new Predmet("Sistemsko programiranje",5,35));
        assertFalse(student.upisiSemestar(studij.getSemestri("RI").get(1),izborniPredmeti));
    }

    @Test
    void predmetPolozen() {
        Student student = new Student("Emir","Hodžić",fakultet,studij,"RI");
        List<Predmet> izborniPredmeti=new ArrayList<>();
        izborniPredmeti.add(new Predmet("Numerički algoritmi",5,35));
        student.upisiSemestar(studij.getSemestri("RI").get(1),izborniPredmeti);
        student.predmetPolozen(studij.getSemestri("RI").get(1).getObavezniPredmeti().get(0),7);
        assertEquals(7,student.dajOcjene().get(studij.getSemestri("RI").get(1).getObavezniPredmeti().get(0)));
    }

    @Test
    void dajOcjene() {
        Student student = new Student("Emir","Hodžić",fakultet,studij,"RI");
        List<Predmet> izborniPredmeti=new ArrayList<>();
        izborniPredmeti.add(new Predmet("Numerički algoritmi",5,35));
        student.upisiSemestar(studij.getSemestri("RI").get(1),izborniPredmeti);
        student.predmetPolozen(studij.getSemestri("RI").get(1).getObavezniPredmeti().get(0),7);
        student.predmetPolozen(studij.getSemestri("RI").get(1).getObavezniPredmeti().get(2),8);
        assertEquals(7,student.dajOcjene().get(studij.getSemestri("RI").get(1).getObavezniPredmeti().get(0)));
        assertEquals(8,student.dajOcjene().get(studij.getSemestri("RI").get(1).getObavezniPredmeti().get(2)));
    }

}