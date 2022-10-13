package ba.unsa.etf.rpr;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class FakultetTest {
    Studij studij;

    FakultetTest() {
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
    }

    @Test
    void getNaziv() {
        Fakultet fakultet=new Fakultet("ETF");
        assertEquals("ETF", fakultet.getNaziv());
    }

    @Test
    void setNaziv() {
        Fakultet fakultet= new Fakultet("ETF");
        fakultet.setNaziv("EFSA");
        assertEquals("EFSA",fakultet.getNaziv());
    }

    @Test
    void getIndeks() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        Student student=new Student("Ana","Marković",fakultet,studij,"RI");
        Student student1=new Student("Marko","Anić",fakultet,studij,"RI");
        assertEquals(3,fakultet.getIndeks());
    }

    @Test
    void getStudiji() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        assertEquals("Prvi ciklus studija",fakultet.getStudiji().get(0).getNaziv());
    }

    @Test
    void dodajStudij() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        assertEquals("Prvi ciklus studija",fakultet.getStudiji().get(0).getNaziv());
    }

    @Test
    void izbaciStudij() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        fakultet.izbaciStudij(studij);
        assertEquals(0, fakultet.getStudiji().size());
    }

    @Test
    void getStudenti() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        Student student=new Student("Ana","Marković",fakultet,studij,"RI");
        Student student1=new Student("Marko","Anić",fakultet,studij,"RI");
        assertEquals("Ana",fakultet.getStudenti().get(0).getIme());
        assertEquals("Marko",fakultet.getStudenti().get(1).getIme());
    }

    @Test
    void ispisiStudenta() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        Student student=new Student("Ana","Marković",fakultet,studij,"RI");
        Student student1=new Student("Marko","Anić",fakultet,studij,"RI");
        fakultet.ispisiStudenta(student);
        assertEquals("Marko",fakultet.getStudenti().get(0).getIme());
    }

    @Test
    void getProfesori() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        Profesor profesor=new Profesor("Željko","Jurić");
        fakultet.dodajProfesora(profesor);
        assertEquals("Željko",fakultet.getProfesori().get(0).getIme());
    }

    @Test
    void dodajProfesora() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        Profesor profesor=new Profesor("Željko","Jurić");
        fakultet.dodajProfesora(profesor);
        assertEquals("Željko",fakultet.getProfesori().get(0).getIme());
    }

    @Test
    void izbaciProfesora() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        Profesor profesor=new Profesor("Željko","Jurić");
        fakultet.dodajProfesora(profesor);
        fakultet.izbaciProfesora(profesor);
        assertEquals(0, fakultet.getProfesori().size());
    }

    @Test
    void dajProfesoreBezNormeIliPrekoNorme() {
        Fakultet fakultet=new Fakultet("ETF");
        fakultet.dodajStudij(studij);
        Profesor profesor=new Profesor("Željko","Jurić");
        profesor.dodajPredmet(new Predmet("Diskretna matematika",5,39));
        fakultet.dodajProfesora(profesor);
        assertEquals("Željko",fakultet.dajProfesoreBezNormeIliPrekoNorme().get(0).getIme());
    }
}