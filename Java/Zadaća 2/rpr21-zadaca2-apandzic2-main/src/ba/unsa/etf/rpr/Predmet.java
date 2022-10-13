package ba.unsa.etf.rpr;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class Predmet {
    private String naziv;
    private int ects;
    private int brojCasovaUSemestru;
    private Profesor profesor;
    private List<Student> studenti;

    public Predmet(String naziv, int ects, int brojCasovaUSemestru) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Naziv nije ispravan!");
        if(ects<=0 || ects>30) throw new IllegalArgumentException("ECTS nije ispravan!");
        if(brojCasovaUSemestru<=0) throw new IllegalArgumentException("Broj časova u semestru nije ispravan!");
        this.naziv = naziv;
        this.ects = ects;
        this.brojCasovaUSemestru=brojCasovaUSemestru;
        studenti=new ArrayList<>();
    }

    public String getNaziv() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Naziv nije ispravan!");
        this.naziv = naziv;
    }

    public int getEcts() {
        return ects;
    }

    public void setEcts(int ects) {
        if(ects<=0 || ects>30) throw new IllegalArgumentException("ECTS nije ispravan!");
        this.ects = ects;
    }

    public int getBrojCasovaUSemestru() {
        return brojCasovaUSemestru;
    }

    public void setBrojCasovaUSemestru(int brojCasovaUSemestru) {
        if(brojCasovaUSemestru<=0) throw new IllegalArgumentException("Broj časova u semestru nije ispravan!");
        this.brojCasovaUSemestru=brojCasovaUSemestru;
    }

    public Profesor getProfesor() {
        if(profesor==null) throw new IllegalArgumentException("Predmet nema profesora!");
        return profesor;
    }

    public void setProfesor(Profesor profesor) {
        this.profesor = profesor;
    }

    public List<Student> getStudenti() {
        if(studenti==null) throw new IllegalArgumentException("Predmet nema upisanih studenata!");
        return studenti;
    }

    public void upisiStudenta(Student student) {
        if(student==null) throw new IllegalArgumentException("Student nije ispravan!");
        if(studenti.contains(student)) throw new IllegalArgumentException("Student je već na ovom predmetu!");
        studenti.add(student);
    }

    public void ispisiStudenta(Student student) {
        if(student==null) throw new IllegalArgumentException("Student nije ispravan!");
        if(!studenti.remove(student)) throw new IllegalArgumentException("Student nije na ovom predmetu!");
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Predmet predmet = (Predmet) o;
        return ects == predmet.ects && naziv.equals(predmet.naziv) && brojCasovaUSemestru==predmet.brojCasovaUSemestru && Objects.equals(profesor,predmet.profesor) && Objects.equals(studenti, predmet.studenti);
    }

    @Override
    public int hashCode() {
        return Objects.hash(naziv);
    }
}
