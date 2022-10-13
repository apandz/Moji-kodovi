package ba.unsa.etf.rpr;

import java.util.*;

public class Pitanje {
    private String tekst;
    private double brojPoena;
    private Map<String, Odgovor> odgovori;

    public Pitanje(String tekst, double brojPoena) {
        this.tekst = tekst;
        this.brojPoena = brojPoena;
        odgovori = new HashMap<>();
    }

    public String getTekst() {
        return tekst;
    }

    public void setTekst(String tekst) {
        this.tekst = tekst;
    }

    public double getBrojPoena() {
        return brojPoena;
    }

    public void setBrojPoena(double brojPoena) {
        this.brojPoena = brojPoena;
    }

    public Map<String, Odgovor> getOdgovori() {
        return odgovori;
    }

    public void setOdgovori(Map<String, Odgovor> odgovori) {
        this.odgovori = odgovori;
    }

    public void dodajOdgovor(String id, String tekstOdgovora, boolean tacno) {
        if(odgovori.containsKey(id)) throw new IllegalArgumentException("Id odgovora mora biti jedinstven");
        odgovori.put(id, new Odgovor(tekstOdgovora, tacno));
    }

    public void obrisiOdgovor(String id) {
        if (!odgovori.containsKey(id)) throw new IllegalArgumentException("Odgovor s ovim id-em ne postoji");
        odgovori.remove(id);
    }

    public List<Odgovor> dajListuOdgovora() {
        List<Odgovor> odgovori1 = new ArrayList<>();
        for (var entry : odgovori.entrySet())
            odgovori1.add(entry.getValue());
        return odgovori1;
    }

    public double izracunajPoene(List<String> ids, SistemBodovanja sistemBodovanja) {
        List<String> brojIds = new ArrayList<>();
        for (String odabraniId : ids) {
            if (!odgovori.containsKey(odabraniId))
                throw new IllegalArgumentException("Odabran je nepostojeći odgovor");
            if (brojIds.contains(odabraniId))
                throw new IllegalArgumentException("Postoje duplikati među odabranim odgovorima");
            brojIds.add(odabraniId);
        }
        double poeni = 0;
        if(!ids.isEmpty()) {
            List<String> tacni = new ArrayList<>(), netacni=new ArrayList<>();
            for (var entry : odgovori.entrySet()) {
                if (entry.getValue().isTacno()) tacni.add(entry.getKey());
                else netacni.add(entry.getKey());
            }
            if (new HashSet<>(tacni).equals(new HashSet<>(ids)))
                poeni=brojPoena;
            else {
                if (sistemBodovanja == SistemBodovanja.PARCIJALNO) {
                    int brojTacnih=0;
                    boolean netacan=false;
                    for(String id: ids){
                        if(netacni.contains(id)) {
                            netacan = true;
                            break;
                        }
                        if(tacni.contains(id)) brojTacnih=brojTacnih+1;
                    }
                    if(!netacan) poeni=brojPoena*brojTacnih*1./odgovori.size();
                } else if (sistemBodovanja == SistemBodovanja.PARCIJALNO_SA_NEGATIVNIM) {
                    int brojTacnih=0;
                    boolean netacan=false;
                    for(String id: ids){
                        if(netacni.contains(id)) {
                            netacan = true;
                            break;
                        }
                        if(tacni.contains(id)) brojTacnih=brojTacnih+1;
                    }
                    if(netacan)
                        poeni=brojPoena/(-2.);
                    else
                        poeni=brojPoena*brojTacnih*1./odgovori.size();
                }
            }
        }
        return poeni;
    }

    @Override
    public String toString() {
        StringBuilder pitanje = new StringBuilder(tekst + "(" + brojPoena + "b)");
        for (var entry : odgovori.entrySet())
            pitanje.append("\n\t").append(entry.getKey()).append(": ").append(entry.getValue().getTekstOdgovora());
        return pitanje.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Pitanje pitanje = (Pitanje) o;
        return Double.compare(pitanje.brojPoena, brojPoena) == 0 && tekst.equals(pitanje.tekst) && Objects.equals(odgovori, pitanje.odgovori);
    }

    @Override
    public int hashCode() {
        return Objects.hash(tekst, brojPoena, odgovori);
    }
}
