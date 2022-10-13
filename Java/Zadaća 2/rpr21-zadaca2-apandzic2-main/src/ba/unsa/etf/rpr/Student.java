package ba.unsa.etf.rpr;

import java.util.List;
import java.util.Map;
import java.util.Objects;

public class Student {
    private String ime;
    private String prezime;
    private Indeks indeks;

    public Student(String ime, String prezime, Fakultet fakultet, Studij studij, String smjer) {
        if(ime.isBlank()) throw new IllegalArgumentException("Ime nije ispravno!");
        if(prezime.isBlank()) throw new IllegalArgumentException("Prezime nije ispravno!");
        if(fakultet==null) throw new IllegalArgumentException("Fakultet nije ispravan!");
        if(studij==null) throw new IllegalArgumentException("Studij nije ispravan!");
        this.ime = ime;
        this.prezime = prezime;
        indeks=new Indeks(fakultet.getIndeks(),fakultet,studij,smjer);
        fakultet.upisiStudenta(this);
    }

    public String getIme() {
        return ime;
    }

    public String getPrezime() {
        return prezime;
    }

    public Indeks getIndeks() {
        return indeks;
    }

    public void promijeniIndeks(Fakultet fakultet, Studij studij, String smjer) {
        if(fakultet==null) throw new IllegalArgumentException("Fakultet nije ispravan!");
        if(studij==null) throw new IllegalArgumentException("Studij nije ispravan!");
        if(fakultet==this.indeks.getFakultet()) {
            indeks=new Indeks(this.indeks.getBrojIndeksa(),fakultet,studij,smjer);
        } else {
            indeks.getFakultet().ispisiStudenta(this);
            indeks = new Indeks(fakultet.getIndeks(), fakultet, studij, smjer);
            fakultet.upisiStudenta(this);
        }
    }

    public boolean upisiSemestar(Semestar semestar, List<Predmet> izborniPredmeti) {
        if(semestar==null) throw new IllegalArgumentException("Semestar nije ispravan!");
        if(!indeks.upisiSemestar(semestar,izborniPredmeti)) return false;
        for(Predmet predmet: semestar.getObavezniPredmeti()) {
            predmet.upisiStudenta(this);
        }
        for(Predmet predmet: izborniPredmeti) {
            predmet.upisiStudenta(this);
        }
        return true;
    }

    public void predmetPolozen(Predmet predmet, Integer ocjena) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if(ocjena<6 || ocjena>10) throw new IllegalArgumentException("Ocjena neispravna!");
        predmet.ispisiStudenta(this);
        indeks.predmetPolozen(predmet,ocjena);
    }

    public Map<Predmet,Integer> dajOcjene() {
        return indeks.dajOcjene();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Student student = (Student) o;
        return ime.equals(student.ime) && prezime.equals(student.prezime) && indeks.equals(student.indeks);
    }

    @Override
    public int hashCode() {
        return Objects.hash(ime, prezime);
    }
}
