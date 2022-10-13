package ba.unsa.etf.rpr;

import java.util.*;

public class Studij {
    private String naziv;
    private int trajanjeStudija;
    private Map<String,List<Semestar>> semestriPoSmjerovima;

    public Studij(String naziv, int trajanjeStudija) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Naziv nije ispravan!");
        if(trajanjeStudija<=0) throw new IllegalArgumentException("Trajanje studija ne može biti negativno!");
        this.naziv = naziv;
        this.trajanjeStudija = trajanjeStudija;
        semestriPoSmjerovima=new HashMap<>();
    }

    public String getNaziv() {
        return naziv;
    }

    public void setNaziv(String naziv) {
        if(naziv.isBlank()) throw new IllegalArgumentException("Naziv nije ispravan!");
        this.naziv = naziv;
    }

    public int getTrajanjeStudija() {
        return trajanjeStudija;
    }

    public void setTrajanjeStudija(int trajanjeStudija) {
        if(trajanjeStudija<=0) throw new IllegalArgumentException("Trajanje studija ne može biti negativno!");
        this.trajanjeStudija = trajanjeStudija;
    }

    public List<Semestar> getSemestri(String smjer) {
        return semestriPoSmjerovima.get(smjer);
    }

    public void dodajSemestar(String smjer, Semestar semestar) {
        if(semestar==null) throw new IllegalArgumentException("Semestar nije ispravan!");
        String smjer1=smjer;
        if(smjer.isBlank()) smjer1="/";
        if(!semestriPoSmjerovima.containsKey(smjer1)) semestriPoSmjerovima.put(smjer1,new ArrayList<Semestar>());
        semestriPoSmjerovima.get(smjer1).add(semestar);
    }

    public boolean izbaciSemestar(String smjer, Semestar semestar) {
        if(semestar==null) throw new IllegalArgumentException("Semestar nije ispravan!");
        String smjer1=smjer;
        if(smjer.isBlank()) smjer1="/";
        if(!semestriPoSmjerovima.containsKey(smjer1)) return false;
        return semestriPoSmjerovima.get(smjer1).remove(semestar);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Studij studij = (Studij) o;
        return trajanjeStudija == studij.trajanjeStudija && naziv.equals(studij.naziv) && Objects.equals(semestriPoSmjerovima, studij.semestriPoSmjerovima);
    }

    @Override
    public int hashCode() {
        return Objects.hash(naziv);
    }

}
