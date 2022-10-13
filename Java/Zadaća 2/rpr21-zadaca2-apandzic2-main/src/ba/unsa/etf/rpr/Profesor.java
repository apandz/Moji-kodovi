package ba.unsa.etf.rpr;

import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public class Profesor {
    private String ime;
    private String prezime;
    private int norma;
    private List<Predmet> predmeti;

    public Profesor(String ime, String prezime) {
        if(ime.isBlank()) throw new IllegalArgumentException("Ime je neispravno!");
        if(prezime.isBlank()) throw new IllegalArgumentException("Prezime je neispravno!");
        this.ime = ime;
        this.prezime = prezime;
        norma=0;
        predmeti=new ArrayList<>();
    }

    public String getIme() {
        return ime;
    }

    public String getPrezime() {
        return prezime;
    }

    public int getNorma() {
        return norma;
    }

    public List<Predmet> getPredmeti() {
        return predmeti;
    }

    public void dodajPredmet(Predmet predmet) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if (predmeti.contains(predmet)) throw new IllegalArgumentException("Profesor je već na ovom predmetu!");
        predmeti.add(predmet);
        norma=norma+predmet.getBrojCasovaUSemestru();
        predmet.setProfesor(this);
    }

    public void izbaciPredmet(Predmet predmet) {
        if(predmet==null) throw new IllegalArgumentException("Predmet je neispravan!");
        if (!predmeti.contains(predmet)) throw new IllegalArgumentException("Profesor nije na ovom predmetu!");
        norma=norma-predmet.getBrojCasovaUSemestru();
        predmet.setProfesor(null);
        predmeti.remove(predmet);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Profesor profesor = (Profesor) o;
        return norma == profesor.norma && ime.equals(profesor.ime) && prezime.equals(profesor.prezime) && Objects.equals(predmeti, profesor.predmeti);
    }

    @Override
    public int hashCode() {
        return Objects.hash(ime, prezime,norma);
    }

}
